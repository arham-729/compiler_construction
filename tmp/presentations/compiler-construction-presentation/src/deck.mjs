const {
  Presentation,
  PresentationFile,
  row,
  column,
  grid,
  rule,
  text,
  fill,
  hug,
  fixed,
  wrap,
  fr,
  auto,
} = await import("@oai/artifact-tool");

const presentation = Presentation.create({
  slideSize: { width: 1920, height: 1080 },
});

const colors = {
  ink: "#13202B",
  muted: "#55606F",
  accent: "#0F766E",
  accentSoft: "#D7F3EE",
};

function base(slide, content) {
  slide.compose(
    column(
      {
        name: "root",
        width: fill,
        height: fill,
        padding: { x: 96, y: 76 },
        gap: 28,
      },
      content,
    ),
    {
      frame: { left: 0, top: 0, width: 1920, height: 1080 },
      baseUnit: 8,
    },
  );
}

function sectionHeader(kicker, title, subtitle) {
  return column(
    {
      name: "section-header",
      width: fill,
      height: hug,
      gap: 18,
    },
    [
      text(kicker, {
        name: "kicker",
        width: fill,
        height: hug,
        style: { fontSize: 20, bold: true, color: colors.accent },
      }),
      text(title, {
        name: "title",
        width: fill,
        height: hug,
        style: { fontSize: 52, bold: true, color: colors.ink },
      }),
      rule({ name: "title-rule", width: fixed(240), stroke: colors.accent, weight: 4 }),
      text(subtitle, {
        name: "subtitle",
        width: wrap(1320),
        height: hug,
        style: { fontSize: 24, color: colors.muted },
      }),
    ],
  );
}

function bulletLine(value) {
  return text(value, {
    width: fill,
    height: hug,
    style: { fontSize: 26, color: colors.ink },
  });
}

{
  const slide = presentation.slides.add();
  base(slide, [
    text("Compiler Construction Project", {
      name: "cover-title",
      width: wrap(1280),
      height: hug,
      style: { fontSize: 68, bold: true, color: colors.ink },
    }),
    rule({ name: "cover-rule", width: fixed(280), stroke: colors.accent, weight: 6 }),
    text(
      "A modular compiler for a defined subset of C with lexical analysis, parsing, semantic validation, optimization, and TAC generation.",
      {
        name: "cover-subtitle",
        width: wrap(1360),
        height: hug,
        style: { fontSize: 30, color: colors.muted },
      },
    ),
    text(
      "Spring 2026 submission deck\nRepository deliverables refreshed in April 2026",
      {
        name: "cover-meta",
        width: wrap(780),
        height: hug,
        style: { fontSize: 22, color: colors.accent },
      },
    ),
  ]);
}

{
  const slide = presentation.slides.add();
  base(slide, [
    sectionHeader(
      "Project brief",
      "What the project had to deliver",
      "The course brief asked for a working compiler for a selected subset of C, a report, test cases, an executable build path, and a presentation-ready explanation of the design.",
    ),
    grid(
      {
        name: "scope-grid",
        width: fill,
        height: hug,
        columns: [fr(1), fr(1)],
        rows: [auto, auto, auto],
        columnGap: 48,
        rowGap: 18,
      },
      [
        bulletLine("Required phases: lexer, parser, semantic analysis, and TAC."),
        bulletLine("Subset: expressions, control flow, functions, and scoped symbols."),
        bulletLine("Optional features implemented: arrays and optimization."),
        bulletLine("Deliverables now included: grammar doc, test docs, demo checklist, and deck."),
        bulletLine("Executable code ships with generated lexer/parser sources for GCC-only rebuilds."),
        bulletLine("Automated regression suite now matches across PowerShell and Bash."),
      ],
    ),
  ]);
}

{
  const slide = presentation.slides.add();
  base(slide, [
    sectionHeader(
      "Architecture",
      "Compiler pipeline",
      "The implementation is split into focused source files so each compiler phase is easy to explain, test, and evolve.",
    ),
    grid(
      {
        name: "pipeline-grid",
        width: fill,
        height: hug,
        columns: [fr(1), fr(1)],
        rows: [auto, auto, auto],
        columnGap: 54,
        rowGap: 26,
      },
      [
        text("1. lexer.l\nTokenizes keywords, identifiers, literals, operators, and delimiters.", {
          width: fill,
          height: hug,
          style: { fontSize: 24, bold: true, color: colors.ink },
        }),
        text("2. parser.y\nBuilds the AST using explicit precedence levels for logical, relational, additive, and multiplicative expressions.", {
          width: fill,
          height: hug,
          style: { fontSize: 24, bold: true, color: colors.ink },
        }),
        text("3. semantic.c + symtab.c\nChecks declarations, function arity, array misuse, and constant bounds while managing global and function-local scopes.", {
          width: fill,
          height: hug,
          style: { fontSize: 24, bold: true, color: colors.ink },
        }),
        text("4. optimize.c\nApplies constant folding, unary folding, and dead-code cleanup before backend emission.", {
          width: fill,
          height: hug,
          style: { fontSize: 24, bold: true, color: colors.ink },
        }),
        text("5. ir.c\nGenerates Three-Address Code for expressions, arrays, control flow, function calls, and returns.", {
          width: fill,
          height: hug,
          style: { fontSize: 24, bold: true, color: colors.ink },
        }),
        text("6. main.c\nDrives parse -> semantic -> optimize -> TAC and stops early when semantic errors are found.", {
          width: fill,
          height: hug,
          style: { fontSize: 24, bold: true, color: colors.ink },
        }),
      ],
    ),
  ]);
}

{
  const slide = presentation.slides.add();
  base(slide, [
    sectionHeader(
      "Language subset",
      "Grammar and semantic guarantees",
      "The repo now includes a report-ready grammar summary and an updated feature audit that matches the current implementation.",
    ),
    grid(
      {
        name: "language-grid",
        width: fill,
        height: hug,
        columns: [fr(1), fr(1)],
        rows: [auto, auto],
        columnGap: 56,
        rowGap: 22,
      },
      [
        text("Supported constructs\nint / void\nvariables and assignments\nif / else\nwhile\nfor\nfunctions\n1D and 2D arrays", {
          width: fill,
          height: hug,
          style: { fontSize: 28, color: colors.ink },
        }),
        text("Semantic checks\nredeclaration\nundeclared identifiers\nundefined functions\nfunction-call arity\narray-vs-scalar misuse\nconstant bounds diagnostics", {
          width: fill,
          height: hug,
          style: { fontSize: 28, color: colors.ink },
        }),
        text("Precedence ladder\n||\n&&\nrelational operators\n+ and -\n* and /\nunary !", {
          width: fill,
          height: hug,
          style: { fontSize: 28, color: colors.ink },
        }),
        text("Current limitations\nno target-code generation\nno nested block-scope symbol table\nconstant-only bounds checks\nint-focused type system", {
          width: fill,
          height: hug,
          style: { fontSize: 28, color: colors.ink },
        }),
      ],
    ),
  ]);
}

{
  const slide = presentation.slides.add();
  base(slide, [
    sectionHeader(
      "Backend result",
      "Optimization and TAC output",
      "A simple demo program is enough to show correct precedence, optimization, and intermediate-code generation.",
    ),
    grid(
      {
        name: "backend-grid",
        width: fill,
        height: hug,
        columns: [fr(1), fr(1)],
        rows: [auto],
        columnGap: 60,
      },
      [
        column(
          { width: fill, height: hug, gap: 16 },
          [
            text("Input", {
              width: fill,
              height: hug,
              style: { fontSize: 24, bold: true, color: colors.accent },
            }),
            text(
              "int main() {\n    int x;\n    x = 5 + 3 * 2 - 1;\n    return x;\n}",
              {
                width: fill,
                height: hug,
                style: { fontSize: 24, color: colors.ink },
              },
            ),
          ],
        ),
        column(
          { width: fill, height: hug, gap: 16 },
          [
            text("Key outcome", {
              width: fill,
              height: hug,
              style: { fontSize: 24, bold: true, color: colors.accent },
            }),
            text(
              "Semantic analysis succeeds.\nOptimizer folds the expression to 10.\nTAC emits a clean return without duplicate terminators.",
              {
                width: fill,
                height: hug,
                style: { fontSize: 26, color: colors.ink },
              },
            ),
          ],
        ),
      ],
    ),
  ]);
}

{
  const slide = presentation.slides.add();
  base(slide, [
    sectionHeader(
      "Validation",
      "Requirements review after the repo refresh",
      "All repository-based requirements from the brief are now covered, and the remaining live assessment items have supporting materials for presentation and demo preparation.",
    ),
    grid(
      {
        name: "validation-grid",
        width: fill,
        height: hug,
        columns: [fr(1), fr(1)],
        rows: [auto, auto, auto],
        columnGap: 56,
        rowGap: 18,
      },
      [
        bulletLine("41/41 automated PowerShell regression checks pass."),
        bulletLine("Bash test runner now mirrors the same coverage."),
        bulletLine("Compilation guide now supports GCC-only rebuilds."),
        bulletLine("Grammar specification added for the written report."),
        bulletLine("Feature audit and test docs now match the real implementation."),
        bulletLine("Demo checklist added for the live walkthrough and viva."),
      ],
    ),
  ]);
}

const pptxBlob = await PresentationFile.exportPptx(presentation);
await pptxBlob.save(new URL("../output/output.pptx", import.meta.url));
