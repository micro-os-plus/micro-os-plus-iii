---
name: code-review
description: 'Perform a thorough, uncompromising code review of the C++ sources, headers, and documentation of the µOS++ IIIe project (a path, a set of commits, or the whole repository): style, naming, documentation, conditional compilation, modern C++, correctness, and tests. Report the problems found in a new numbered report, each with a permanent number. Use when the user asks for a "code review", to "review the code", to "review changes", or to check a file, folder, or pull request for style, naming, documentation, and correctness issues.'
argument-hint: 'Optional: path, folder, or commit range to review; defaults to include/, src/, and tests/'
---

<!--
DO NOT EDIT! Automatically generated from template file:
npm-packages-helper/templates/common/.github/skills/_micro-os-plus/code-review/SKILL-liquid.md;
put the project rules in .github/skills/project.md (all the skills) or
in .github/skills/code-review/project.md (this skill only);
see .github/skills/README.md.
-->

# Code Review — µOS++ IIIe

## When to Use

- The user asks for a code review of a file, folder, diff, set of
  commits, or the whole repository.
- The user asks to check adherence to the project's coding standards
  before a commit or release.

This skill reviews the code against the project's conventions. The
design of the library as a whole (the public API, the structure) is
reviewed by the `design-review` skill, and the Doxygen comments in
depth by `doxygen-review`; suggest them when a question of that kind
arises, rather than extending this review.

Read `.github/skills/review-common.md` (the rules shared by the review
skills) and `.github/skills/project-defaults.md`, `.github/skills/project.md` first, and `project.md` in this
folder, if it exists (for the project-specific checks); their rules
extend these, and take precedence where they differ.

## Review Scope

- If the user specifies a path, review only that path, in full.
- If the user specifies commits or a range, review their diff
  (`git diff <range>`), reading the changed files in full for context.
- Otherwise review `include/`, `src/`, and `tests/` (excluding
  `tests/build/`).

Do not review the generated files (those with a `DO NOT EDIT!`
header); report a problem in them against their template.

State the scope, the branch, and the commit (`git rev-parse --short
HEAD`) at the top of the report.

## Checklist

Review every item below. Do not soften findings; state flaws directly and
specifically, with file and line references.

### Naming and style

- C/C++ identifiers use `snake_case` (types, functions, variables, members).
- File names are lower-case words separated by dashes (for example
  `<name>-inlines.h`).
- Formatting matches `maintenance/config/.clang-format` (see "Tooling
  Verification").
- No lines exceed 80 characters, including comments, except the file
  path in a `DO NOT EDIT` generated-file comment, and link URLs.
- The change follows the patterns already used elsewhere in the code
  base, or has a visible reason not to.

### Folder structure

- Declarations live in `include/`, out-of-line definitions in `src/`,
  inline (header-only) definitions in `inlines/`.
- Usually there is only one public header (for example
  `include/micro-os-plus/<name>.h`); the internal headers are located
  in a subfolder named after the public header (for example
  `include/micro-os-plus/<name>/`).
- New source files are registered in `xcdl-package.jsonc`, and the
  top-level `CMakeLists.txt` and `meson.build` are regenerated from it
  with `xpm run xcdl-export`; the generated files are not edited by hand
  (see the `xcdl` skill).

### Documentation (Doxygen)

- Every class, method, property, parameter, and return value has a
  Doxygen comment, including `private` and `protected` members, as
  required by the rules of the `doxygen-review` skill.
- Each declaration/definition has both `@brief` and `@details`.
- `@details` expands on `@brief` rather than repeating it; it is located
  immediately after `@brief` in the comment block, or, if the declaration
  and definition are in separate files, in the comment immediately before
  the definition.
- `@details` provides a more in-depth explanation, including any
  relevant information about the implementation, usage, or edge cases.
- Documentation uses British English spelling (for example
  "behaviour", "colour", "initialise") and formal, contraction-free
  wording.
- If the code already includes documentation, review and possibly improve it.
- If Doxygen can be run, check its warning log (undocumented parameters,
  `@ref`/`@tparam` targets that do not resolve, mismatched signatures)
  rather than relying solely on a manual read.

### Licensing and metadata

- Every source and header file carries the identical MIT licence header
  block, with a plausible copyright year range for the project.
- If a change is significant enough to affect behaviour, check whether
  `CHANGELOG.md` and the package version should be updated.

### Conditional compilation

- Header guards are the only construct permitted to use `#ifndef`.
- All other conditional compilation uses `#if defined(...)` or
  `#if !defined(...)`, never bare `#ifdef` / `#ifndef`.
- Every `#endif` is followed by a C++ comment (`// ...`) repeating the
  exact expression from its paired `#if`/`#if defined`/`#if !defined`,
  so nested and distant guards remain traceable.
- The above applies to pragmas where `#if defined(__GNUC__)`,
  `#if defined(__clang__)` or similar are used.

### Modern C++

- The codebase targets C++20; flag code written in an older style where
  a modern equivalent is clearly preferable.
- Prefer `concepts` and `requires` clauses over SFINAE and
  `std::enable_if`.
- Prefer `constexpr`/`consteval` over macros and runtime checks where the
  value is known at compile time.
- Prefer range-based `for`, structured bindings, `auto`, and `<=>`
  (three-way comparison) where they simplify the code without harming
  readability.
- Prefer `[[nodiscard]]`, `[[maybe_unused]]`, and other standard
  attributes over compiler-specific equivalents.
- Flag unnecessary use of raw pointers, casts, or macros where a
  standard C++20 facility would be clearer and safer, while respecting
  the embedded/bare-metal constraints of the project (no exceptions,
  no heap allocation, no RTTI, unless already used elsewhere).
- `noexcept` is specified explicitly wherever it is warranted (for
  example move constructors/assignment, destructors, and functions that
  provably cannot throw); flag missing or incorrect `noexcept`.

### Correctness and maintainability

- Look for undefined behaviour, lifetime and ownership issues, missing
  `const`-correctness, and incorrect inline assembly (wrong mnemonics
  or operand direction, missing clobbers or `memory` barriers, operands
  of the wrong width for the registers).
- Check for potential ABI or layout implications of changes to the
  public types and to the interfaces shared with other components.
- Flag weak assumptions, missing edge-case handling, and any deviation
  from existing patterns in the codebase without justification.
- Check that thread-safety/reentrancy expectations are documented where
  relevant (any function that could be called from an interrupt or
  exception handler, from multiple threads, or from multiple cores
  should say so explicitly).
- Check API symmetry between the parallel forms of each API (for
  example the C functions and the C++ namespaces, or related class
  templates); every function should exist in all forms, with consistent
  names, unless the omission is deliberate and justified, and the
  justification is noted.

### Tests

- Changes to behaviour are covered by tests in `tests/`.
- Note which of the tests listed under "Tests" in
  `.github/skills/project.md` should be re-run.

## Tooling Verification

Where feasible, verify claims by actually building rather than reading
only:

- run the tests listed under "Tests" in `.github/skills/project.md`
  (they build with `-Wall -Wextra -Wpedantic` and stricter warnings),
  and report any warnings, rather than only flagging style issues that
  look suspicious;
- check the formatting without modifying the files:

  ```sh
  xpacks/.bin/clang-format --dry-run --Werror \
    --style=file:maintenance/config/.clang-format <files>
  ```

- for pointer-arithmetic or type-punning idioms (for example
  `reinterpret_cast` member-offset tricks, null-pointer-based
  `offsetof` substitutes), build a probe with
  `-fsanitize=undefined`/`-fsanitize=address`, if available, and flag
  any undefined behaviour it reports.

## Evidence

Probe the suspected defects as described under "Confirming Findings
with Probes" in `.github/skills/review-common.md`, and tag the
confirmed ones **[confirmed]**.

## Report

Write the report to a new file, `maintenance/docs/CODE-REVIEW-<n>.md`
(see "Report Files" in `.github/skills/review-common.md`):

```markdown
# Code review

Date: <YYYY-MM-DD HH:MM>, on branch `<branch>`, commit `<hash>`.

Scope: <the path, the range, or the default scope>.

Tooling: <the tests run and their results, the formatting check, the
sanitizers>.

Summary: <n> problems. <The most serious, by number, and specific,
actionable recommendations.>

## Naming and style

- **#1 — <short title>.** [internal] **[confirmed]**
  `<file>` line <n>: <what is wrong, the evidence, and the suggested
  change>

## Folder structure

## Documentation

## Licensing and metadata

## Conditional compilation

## Modern C++

## Correctness and maintainability

## Tests

## Not reported

<anything left out because it was not certain enough to include>
```

## Procedure

1. Identify the review scope (explicit path or range, or the default
   scope above), and the number of the new report.
2. Read the relevant files fully; do not review based on partial
   context.
3. Go through the checklist section by section, collecting concrete
   findings (file, line, description, why it matters).
4. Probe the suspected defects that can be probed, and run the tooling.
5. Explicitly note anything left out of the review because you were not
   certain enough to include it ("Not reported").
6. Number the findings (see "Numbering" in
   `.github/skills/review-common.md`), and write the report.
7. Give the user a short summary: the counts and the most serious
   problems by number.
8. Do not fix the issues unless the user asks for that separately; see
   "Acting on an Item" in `.github/skills/review-common.md`.
