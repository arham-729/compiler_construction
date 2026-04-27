#!/usr/bin/env python3
"""Separate Python target-code generator for the compiler project's TAC."""

from __future__ import annotations

import argparse
import re
import sys
from dataclasses import dataclass
from typing import List


FUNC_RE = re.compile(r"^Func\s+([A-Za-z_]\w*):$")
LABEL_RE = re.compile(r"^(L\d+):$")
GOTO_RE = re.compile(r"^goto\s+(L\d+)$")
IF_FALSE_RE = re.compile(r"^if_false\s+(\S+)\s+goto\s+(L\d+)$")
RETURN_RE = re.compile(r"^return(?:\s+(\S+))?$")
PARAM_RE = re.compile(r"^param\s+(\S+)$")
CALL_RE = re.compile(r"^(t\d+)\s*=\s*call\s+([A-Za-z_]\w*),\s*(\d+)$")
ARRAY_STORE_RE = re.compile(r"^([A-Za-z_]\w*)\[(\S+)\]\s*=\s*(\S+)$")
ARRAY_LOAD_RE = re.compile(r"^(t\d+)\s*=\s*([A-Za-z_]\w*)\[(\S+)\]$")
BINOP_RE = re.compile(
    r"^([A-Za-z_]\w*|t\d+)\s*=\s*(\S+)\s*(\|\||&&|==|!=|<=|>=|<|>|\+|-|\*|/)\s*(\S+)$"
)
UNOP_RE = re.compile(r"^([A-Za-z_]\w*|t\d+)\s*=\s*(!)(\S+)$")
COPY_RE = re.compile(r"^([A-Za-z_]\w*|t\d+)\s*=\s*(\S+)$")
HEADING_RE = re.compile(r"^---\s+Generating Intermediate Representation \(TAC\)\s+---$")
SECTION_RE = re.compile(r"^---\s+")


@dataclass
class TacProgram:
    lines: List[str]


def extract_tac_lines(text: str) -> TacProgram:
    raw_lines = [line.rstrip() for line in text.splitlines()]

    if any(HEADING_RE.match(line.strip()) for line in raw_lines):
        in_tac = False
        tac_lines: List[str] = []
        for raw in raw_lines:
            line = raw.strip()
            if HEADING_RE.match(line):
                in_tac = True
                continue
            if in_tac and SECTION_RE.match(line):
                break
            if in_tac:
                tac_lines.append(raw)
        lines = tac_lines
    else:
        lines = raw_lines

    cleaned = [line.strip() for line in lines if line.strip()]
    return TacProgram(cleaned)


def emit_load(value: str, out: List[str]) -> None:
    if re.fullmatch(r"-?\d+", value):
        out.append(f"  PUSHI {value}")
    else:
        out.append(f"  LOAD {value}")


def emit_operator(op: str, out: List[str]) -> None:
    mapping = {
        "+": "ADD",
        "-": "SUB",
        "*": "MUL",
        "/": "DIV",
        "&&": "AND",
        "||": "OR",
        "<": "CMPLT",
        ">": "CMPGT",
        "<=": "CMPLE",
        ">=": "CMPGE",
        "==": "CMPEQ",
        "!=": "CMPNE",
    }
    out.append(f"  {mapping[op]}")


def translate_tac(program: TacProgram) -> str:
    out: List[str] = []
    pending_params: List[str] = []

    for line in program.lines:
        if match := FUNC_RE.match(line):
            if out and out[-1] != "":
                out.append("")
            out.append(f"FUNC {match.group(1)}:")
            out.append("  ; parameters are assumed pre-bound for this TAC-driven backend")
            pending_params.clear()
            continue

        if match := LABEL_RE.match(line):
            out.append(f"{match.group(1)}:")
            continue

        if match := GOTO_RE.match(line):
            out.append(f"  JMP {match.group(1)}")
            continue

        if match := IF_FALSE_RE.match(line):
            emit_load(match.group(1), out)
            out.append(f"  JZ {match.group(2)}")
            continue

        if match := PARAM_RE.match(line):
            pending_params.append(match.group(1))
            continue

        if match := CALL_RE.match(line):
            for arg in pending_params:
                emit_load(arg, out)
            pending_params.clear()
            out.append(f"  CALL {match.group(2)} {match.group(3)}")
            out.append(f"  STORE {match.group(1)}")
            continue

        if match := ARRAY_STORE_RE.match(line):
            emit_load(match.group(2), out)
            emit_load(match.group(3), out)
            out.append(f"  STOREARR {match.group(1)}")
            continue

        if match := ARRAY_LOAD_RE.match(line):
            emit_load(match.group(3), out)
            out.append(f"  LOADARR {match.group(2)}")
            out.append(f"  STORE {match.group(1)}")
            continue

        if match := BINOP_RE.match(line):
            emit_load(match.group(2), out)
            emit_load(match.group(4), out)
            emit_operator(match.group(3), out)
            out.append(f"  STORE {match.group(1)}")
            continue

        if match := UNOP_RE.match(line):
            emit_load(match.group(3), out)
            out.append("  NOT")
            out.append(f"  STORE {match.group(1)}")
            continue

        if match := RETURN_RE.match(line):
            value = match.group(1)
            if value is None:
                out.append("  RET")
            else:
                emit_load(value, out)
                out.append("  RETVAL")
            continue

        if match := COPY_RE.match(line):
            emit_load(match.group(2), out)
            out.append(f"  STORE {match.group(1)}")
            continue

        raise ValueError(f"Unsupported TAC line: {line}")

    return "\n".join(out).rstrip() + "\n"


def main() -> int:
    parser = argparse.ArgumentParser(description="Translate compiler TAC to stack-VM target code.")
    parser.add_argument("path", nargs="?", help="Optional input file. Reads stdin if omitted.")
    args = parser.parse_args()

    if args.path:
        with open(args.path, "r", encoding="utf-8") as handle:
            source = handle.read()
    else:
        source = sys.stdin.read()

    if not source.strip():
        print("No input provided.", file=sys.stderr)
        return 1

    program = extract_tac_lines(source)
    sys.stdout.write(translate_tac(program))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
