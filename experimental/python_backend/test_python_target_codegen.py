import subprocess
import sys
import textwrap
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parent
SCRIPT = ROOT / "python_target_codegen.py"


def run_codegen(source: str) -> str:
    result = subprocess.run(
        [sys.executable, str(SCRIPT)],
        input=source,
        text=True,
        capture_output=True,
        check=True,
    )
    return result.stdout


class PythonTargetCodegenTests(unittest.TestCase):
    def test_translates_compiler_output_section(self) -> None:
        compiler_output = textwrap.dedent(
            """
            --- Generating Intermediate Representation (TAC) ---

            Func main:
            x = 10
            return x

            --- Generating Target Code (Stack VM) ---
            """
        ).strip()

        output = run_codegen(compiler_output)

        self.assertIn("FUNC main:", output)
        self.assertIn("PUSHI 10", output)
        self.assertIn("STORE x", output)
        self.assertIn("RETVAL", output)

    def test_translates_function_call_tac(self) -> None:
        tac = textwrap.dedent(
            """
            Func main:
            param 5
            param 10
            t1 = call add, 2
            return t1
            """
        ).strip()

        output = run_codegen(tac)

        self.assertIn("CALL add 2", output)
        self.assertIn("STORE t1", output)
        self.assertIn("LOAD t1", output)

    def test_translates_control_flow_and_arrays(self) -> None:
        tac = textwrap.dedent(
            """
            Func main:
            L0:
            t1 = i < 5
            if_false t1 goto L1
            t2 = arr[i]
            arr[i] = t2
            goto L0
            L1:
            return
            """
        ).strip()

        output = run_codegen(tac)

        self.assertIn("L0:", output)
        self.assertIn("CMPLT", output)
        self.assertIn("JZ L1", output)
        self.assertIn("LOADARR arr", output)
        self.assertIn("STOREARR arr", output)
        self.assertIn("RET", output)


if __name__ == "__main__":
    unittest.main()
