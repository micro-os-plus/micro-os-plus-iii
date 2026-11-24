# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code)
when working with code in this repository.
It includes the rules common to all projects; the project-specific
rules are in `.claude/project.md`:

@.claude/project.md

<!--
DO NOT EDIT! Automatically generated from template file:
npm-packages-helper/templates/common/CLAUDE-liquid.md.
-->

## Project Overview

This is the **µOS++ IIIe** project, part of
µOS++.

## General

- Avoid sycophantic behaviour; for all conversation, never soften criticism
  to protect the person's ego.
- If something has a flaw, say so directly.
- When the meaning of a question is uncertain, say so and ask questions
  rather than guess.
- When multiple valid answers are possible, say so and ask questions to
  identify the most appropriate one, rather than assuming a single
  correct answer.
- This applies to every response.

## Language and Tone

- Use British English spelling and grammar (e.g., "behaviour", "colour",
  "organise", "analyse", "favour", "initialise", etc.)
- Maintain a professional and formal tone in all generated content
- Avoid colloquialisms, slang, or informal expressions
- Avoid humour, jokes, or casual remarks
- Use clear, precise, and professional language appropriate for technical
  documentation
- Avoid contractions (e.g., use "do not" instead of "don't")
- Use the Oxford comma in lists for clarity
- Maintain consistency in terminology throughout the codebase
- Prefer "folder" to "directory"

## Code Style

- Follow the existing C++ code style defined in the .clang-format file.
- Use consistent formatting and naming conventions based on prettier and
  clang-format configurations.
- For C/C++, the naming convention is snake_case.
- For C++, write multiple level namespaces on the same line.

## Includes order

- Project-specific headers.
- µOS++ headers
- Third-party library headers.
- Standard library headers.
- Use alphabetical order within each group.
- Separate each group with a blank line.
- Brace the whole group of includes by separator lines

## Compiler pragmas

When needed to silence warnings, use separate groups of pragmas for each compiler.

Always use `__GNUC__` guards, and, if necessary, `__clang__` guards to apply
compiler-specific pragmas.

```c++
#if defined(__GNUC__)
#pragma GCC diagnostic ignored "-Waggregate-return"
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wc++98-compat"
#pragma clang diagnostic ignored "-Wc++98-compat-pedantic"
#endif // defined(__clang__)
#pragma GCC diagnostic ignored "-Wredundant-tags"
#endif // defined(__GNUC__)
```

Brace the whole group of pragmas by separator lines.

## Documentation

- Add comprehensive documentation comments accepted by Doxygen
- Document all classes, methods, properties, parameters, and return types.
- The declarations should be in the headers, the definitions in the `src`
  folder, and the inline definitions in the `inlines` folder.
- Add @details sections to all declarations and definitions.
- The @brief and @details sections are displayed sequentially, so ensure the
  @details section expands on the @brief without repeating it. The @brief
  should be a concise summary of the member's purpose, whilst the @details
  should provide a more in-depth explanation, including any relevant
  information about the implementation, usage, or edge cases.
- Document private and protected members as well
- Keep the line length below 80 characters
- If the code already includes documentation, review and possibly improve it.

## Folder Structure

- `/src`: Contains the C++ source code
- `/include`: Contains the C++ header files
- `/tests`: Contains the test suites and test cases
- `/maintenance`: Contains the project maintenance resources, which are
  not published with the package: `/maintenance/config` (the formatter
  configuration files), `/maintenance/scripts` (the maintenance scripts
  and their templates), and `/maintenance/docs` (the developer notes)

When adding new source files, place them in the appropriate `src` or `include`
folder, and add them to `xcdl-package.jsonc`, from which the top CMake and Meson
configurations are generated with `xpm run xcdl-export` (see the xcdl section
below); do not edit the generated files.

Avoid running `find /` commands that search the entire filesystem, as this
always times out.

## Tool Binaries

The compiler and the other binary tools are installed in the
`build/<configuration>/xpacks/.bin` folder of each build configuration,
or in the `<project>/xpacks/.bin` folder of the project root; the
JavaScript tools are in the `node_modules/.bin` folders.

## Testing

After making major changes, run in a terminal:

- `xpm run test-selected -C tests` -- to execute a minimal selection of tests

After minor changes, take a shortcut and run only:

- `xpm run test -C tests` -- to execute the test with the system compiler.

When using linked writable projects, it is necessary to run the linking step to ensure all dependencies are correctly resolved.

- `xpm run link-dependencies --config <name>`

When using non-native platforms, run one by one specific actions for the given configuration.

- `xpm run setup --config <name>`
- `xpm run build --config <name>`

For non-qemu platforms, running the tests can be done only after confirming that the board is
powered up, with the command:

- `xpm run test --config <name>`

QEMU tests can be done directly, without confirmation that the board is
powered up.

## Reviews

When asked for one of the following reviews, follow the separate
instructions of the corresponding skill:

- a design review, or an API review:
  `.github/skills/design-review/SKILL.md`;
- a code review: `.github/skills/code-review/SKILL.md`;
- a review of the Doxygen comments:
  `.github/skills/doxygen-review/SKILL.md`;
- a consistency check: `.github/skills/consistency-check/SKILL.md`.

The review skills share the rules in `.github/skills/review-common.md`,
and all the skills read the facts of this project in
`.github/skills/project.md`. Most skills are generated from the
`npm-packages-helper` templates (`npm run generate-top-commons`), so
the project-specific rules go in `.github/skills/project.md`, or in a
`project.md` file in the folder of the skill, never in a generated
`SKILL.md` (see `.github/skills/README.md`).

## Git Commits

- When asked to commit, or to write a commit message, follow the separate
  instructions in `.github/skills/github-commit/SKILL.md`, which cover the
  commit message format, the GitHub issue references, and the closing
  comment of the GitHub issue.
- Do not commit without user review and approval.

## Issues

- When asked to open an issue, follow the separate instructions in
  `.github/skills/create-github-issue/SKILL.md`.
- After committing a fix for a GitHub issue, add the closing comment to
  that issue, as described in `.github/skills/github-commit/SKILL.md`
  ("Issue Comment").

## xcdl

When working with `xcdl-package.jsonc`, or when adding, removing, or
renaming a source file, an include folder, a compiler or linker option,
or a dependency, follow the separate instructions in
`.github/skills/xcdl/SKILL.md`.

## Verbal Questions

When more input from the user is required, first ask a short version
of the question aloud, with the macOS `say` program, and only then show
the full question in the chat window (in the text of the reply, or in a
question dialog), so that the spoken question draws the attention of
the user before the text appears.
