---
name: design-review
description: 'Review the design of the C++ API and implementation of the µOS++ IIIe project from its source code alone (headers, inlines, and src/, ignoring the Doxygen comments): defects, usability for a new user (programs written from the manual and the public headers alone, and the compiler diagnostics of common mistakes), structure and layering, naming and consistency, simplicity, modern C++ usage, and suitability for embedded targets. Confirm suspected defects with probe programs. Each review is fresh (the earlier reports are not read); report the problems found in a new numbered report, each with a permanent number, and suggest improvements in the source files. Use when the user asks for a "design review", an "API review", an "API and code review", to "review the API", to "analyse the API design", or to review the design or implementation of the library (as a whole, or of a header or folder).'
argument-hint: 'Optional: header, file, or folder to review; defaults to include/ and src/'
---

<!--
DO NOT EDIT!
Automatically generated from template file: npm-packages-helper/templates/common/.github/skills/_micro-os-plus/design-review/SKILL-liquid.md;
put the project rules in .github/skills/project.md (all the skills) or
in .github/skills/design-review/project.md (this skill only);
see .github/skills/README.md.
-->

# Design Review — µOS++ IIIe

## When to Use

- The user asks for a review of the API, or of the design or the
  implementation of some classes or headers.
- Before a major release, to collect the breaking changes worth making.
- After a large change to the public headers, to check that the API is
  still consistent.

This skill judges the API and its implementation as a user and a
maintainer meet them: through the code. It reports problems and
suggests improvements; it does not change the code, unless the user
asks for that separately (for example "fix #4"; see "Acting on an Item"
in `.github/skills/review-common.md`).

Related skills, with different scopes:

- `code-review`: the style, documentation, and correctness of a
  change (a diff or a pull request);
- `doxygen-review`: the Doxygen comments;
- `consistency-check`: the consistency between the code, the website,
  and the other project files;

Read `.github/skills/review-common.md` (the rules shared by the review
skills) and `.github/skills/project-defaults.md`, `.github/skills/project.md` first, and `project.md` in this
folder, if it exists; their rules extend these, and take precedence
where they differ. In particular, `project.md` in this folder lists the
**restrictions**: the deliberate decisions of the design that must not
be reported as problems, and the proposals that must not be made.

## A Fresh Review Each Time

**Each review is independent of the previous ones.** Do not read the
earlier design reviews (`maintenance/docs/DESIGN-REVIEW-*.md`, and the
older names listed under "Review Documents" in
`.github/skills/project.md`), the other review, comparison, and
compatibility reports in `maintenance/docs/`, or the commit messages and
GitHub issues that refer to their items. Judge the code as it is now,
as if for the first time: the earlier conclusions must not decide what
is examined, what is reported, or how it is classified. A problem that
an earlier review already reported, and that is still present, is
reported again.

The only use of the earlier reports is their file names, to choose
the number of the new report (see "Report"). Never modify an earlier
report as part of a new review. The documents that describe a method
(listed in `project.md` in this folder, if any) may be followed, but
their results may not be read.

## Scope

If the user specifies a header, a file, or a folder, review only that
(but read whatever it depends on). Otherwise review:

- the umbrella header (for example `include/micro-os-plus/<name>.h`);
- the public headers in the folder named after it;
- the inline and template definitions in its `inlines/` folder;
- the definitions in `src/`.

The build integration that lists the source files is `xcdl-package.jsonc`;
the top-level `CMakeLists.txt` and `meson.build` are generated from it.

The test programs in `tests/` are not reviewed, but read them to see
how the API is used in practice; they also show which cases the
existing tests cover, and which they do not.

State the scope, the branch, and the commit (`git rev-parse --short
HEAD`) at the top of the report.

## Using the API as a Newcomer

Before reading the implementation, use the library as a new user
would, to find what makes it hard to learn and to use. This exercise
comes first: once the implementation has been read, it can no longer
be done without that knowledge.

If the agent can start a sub-agent, give the exercise to a fresh one,
with only this section (and the list of tasks of `project.md` in this
folder) as its instructions, and use its notes. Otherwise do it before
reading the stripped files.

The newcomer reads only what a user reads: the user manual (on the
website, if any), the umbrella header, and the declarations of the
public headers, comments included (this is the only part of the review
that reads the comments, because the users read them). The newcomer
does not read the declarations in the `detail` namespace, the
`inlines/` folder, `src/`, the tests, or the earlier reports. Every
task that cannot be completed without them is a finding.

Write one small program per task in `<scratch>/newcomer/`, and build
it as the probes are built (see "Confirming Findings with Probes"). The
tasks are listed in `project.md` in this folder; without such a list,
choose the five to seven tasks that a new user of the library would
attempt first, from the simplest use to the configuration for an
embedded target.

For each program, record:

- what had to be looked up, and where it was found, or not found;
- the code that every program repeats (the boilerplate of `main()`,
  the namespaces, and the includes);
- the places where several forms are offered for the same thing, with
  nothing to tell which one to choose;
- the places where the manual and the headers disagree;
- whether the output (or the error) of a failing use tells, without
  the source, what went wrong.

Then make the common mistakes on purpose (listed in `project.md` in
this folder, or, without such a list, the misuses that the
constraints of the API are meant to reject), and build each with GCC
and Clang. For each mistake, record the length of the first
diagnostic, and whether it names the mistake in the user's terms, or
shows only the internals (`detail` types, or a concept that fails deep
in the instantiation stack).

The findings go to section 2 of the checks ("Usability"). A problem
whose only cause is the documentation (the manual or the Doxygen
comments) goes to the incidental observations, with the skill that
covers it (`doxygen-review`).

## Reading the Code

**Ignore the Doxygen comments.** The review judges what the code
says, not what the comments promise; a problem that the comments
explain away is still a problem of the API (the comments are reviewed
by `doxygen-review`). Strip them first, keeping the original line
numbers, so that every finding can cite the real file and line:

```sh
node .github/skills/design-review/scripts/strip-comments.mjs <scratch>/stripped
```

Run it from the repository root; `<scratch>` is the session scratchpad
folder (never write into the repository). Optional arguments select
other files or folders. Each output line starts with its original line
number.

Then read **every** stripped file in full; do not skim and do not rely
on searches alone. Read from the user's entry points inwards: the
umbrella header (what is included, and in which order), then the
classes that the application uses first, then the internal ones, and
the deprecated definitions last; `project.md` in this folder may give
the order for this project.

While reading, note for each class and function: who calls it (the
user, the library, or only the tests), what it costs (allocations,
copies, formatting), and what it does that its name does not say.
Use `grep` over `include/`, `src/`, and `tests/` to find out whether an
entity is used at all.

The stripped files serve the design review only. The checks of the
layout of the files (formatting, line length, header guards) are made
on the original files.

Check that each public header compiles on its own:

```sh
node .github/skills/design-review/scripts/check-headers.mjs <scratch>/headers \
  [<header-folder>...] [-- <flag>...]
```

The compiler is taken from `CXX` (default `c++`); `project.md` in this
folder gives the folders and the flags for this project.

Check the formatting without modifying the files:

```sh
xpacks/.bin/clang-format --dry-run --Werror \
  --style=file:maintenance/config/.clang-format <files>
```

## Confirming Findings with Probes

Follow "Confirming Findings with Probes" in
`.github/skills/review-common.md`, and build the probes as described
under "Probes" in `.github/skills/project.md`. In addition:

- Build with both GCC and Clang when the question involves warnings,
  overload resolution, or name lookup. With Clang, `-Weverything`
  reports much more; report only the warnings that the library's own
  pragmas do not already silence deliberately.
- Use one probe per question, or one probe with `#if defined(P_...)`
  sections compiled one at a time, when the question is whether
  something compiles or links.
- Build once with `-fno-exceptions -fno-rtti` when the question
  concerns the code guarded by `__cpp_exceptions` or anything that
  may depend on RTTI.
- For the questions that depend on the target (the widths of the
  types, the floating-point unit, the stack frames, warnings such as
  `-Wpsabi` that only appear there), compile the library and the probe
  with a 32-bit cross compiler as well (for example
  `arm-none-eabi-g++`, from the `xpacks/.bin/` folder of a test build
  folder, if any). Compiling with `-c` needs no platform files:

  ```sh
  arm-none-eabi-g++ -std=c++20 -mcpu=cortex-m4 -mthumb \
    -mfloat-abi=hard -mfpu=fpv4-sp-d16 -fno-exceptions -fno-rtti \
    -Wall -Wextra -Wpedantic -Wdouble-promotion \
    -Wframe-larger-than=512 -fstack-usage \
    -Iinclude -c <file> -o <scratch>/cross/<name>.o
  ```

- For lifetime questions, compile the probe with
  `-g -fsanitize=address` (and `-fsanitize=undefined` for arithmetic,
  pointer arithmetic, and type punning); use strings long enough to be
  allocated on the heap, otherwise the small string optimisation hides
  the error.

## Checks

### 1. Defects

Behaviour a user would see as wrong. These come first in the report.
The general questions (`project.md` in this folder adds those specific
to this library):

- **Undefined behaviour**: code that compiles but may crash or produce
  incorrect results (null or dangling pointers, out-of-bounds access,
  signed overflow, invalid casts, pointer arithmetic outside an
  object).
- **Lifetimes**: which pointers and references does an object keep
  (names as `const char*`, `string_view` members, references to
  arguments)? Is the stored object used after the call returns?
- **Call order and repetition**: what happens when a function is
  called twice, out of order, or not at all (initialisation, shutdown)?
  Are side effects hidden in functions that look like queries?
- **Shared state**: objects shared by several users (buffers,
  formatters, registries); what happens when two of them are alive at
  once?
- **Name lookup**: unqualified calls to generic names (`get`, `abs`,
  `pow`) inside templates, which argument-dependent lookup can
  redirect to a user's function.
- **Concepts that match too much**: concepts and traits that
  recognise a type by the presence of a member (`value`, `npos`,
  `begin()` and `end()`) also match user types that happen to have a
  member with that name. Probe each one with such a user type and
  check that the result does not change.
- **Special values**: `'\0'`, empty strings, null C strings, NaN, the
  infinities, `-0.0`, and the minimum and maximum of each type; the
  widths of the intermediate types on 32-bit targets (`unsigned long`
  is 32 bits on Cortex-M, RISC-V 32, and AArch32).
- **Exceptions**: what happens when user code called by the library
  throws. With exceptions disabled, the same failures must have the
  same effect, unless the difference is unavoidable and visible.
- **Registration**: objects constructed before the object they
  register with, in other translation units (the static initialisation
  order), or after it; none of them may be silently ignored.
- **Copy and move**: objects that are registered in a list or referred
  to by pointer must not be copyable or movable; value types must be
  copyable. A constructor with side effects (a registration) combined
  with an implicit copy registers twice.
- **Include order and configuration**: a type, a default, or an
  inline body that changes with the order of the includes, or with
  the macros of `micro-os-plus/project-config.h` and the
  `*-defines.h` headers, violates the one definition rule when two
  translation units differ. Probe with two translation units that
  include the headers in different orders, or with different macros,
  linked together.
- **Stack usage**: recursion (also in templates), large local buffers,
  and large objects passed by value; embedded stacks are a few
  kilobytes. Use the `-fstack-usage` output of the cross build.
- **Header self-containment** (the script above).

### 2. Usability

The findings of "Using the API as a Newcomer":

- tasks that cannot be completed from the manual and the public
  declarations alone;
- boilerplate that every program repeats, and that the library could
  provide;
- several forms for the same thing, with nothing to choose between
  them;
- diagnostics of common mistakes that are long, or that name only the
  internals; a constraint or a `static_assert` with a message at the
  boundary of the API often shortens them;
- outputs and errors that do not tell what went wrong.

Each finding cites the newcomer program, and the diagnostic or the
output, as its evidence.

### 3. Structure and layering

- **Public and internal**: which members of the public classes exist
  only for the library (setters, counters, mutable accessors)? Can a
  user break the internal state? Are internal constructors public?
- **Leaks of `detail`**: `detail` types in public signatures, in base
  classes of public classes, or returned through `auto`, which force
  the users to name them.
- **Extensibility**: are the classes that look extensible actually
  extensible, and are the parts meant for users separated from the
  internal protocol?
- **Configuration**: can everything be configured without a command
  line (embedded targets have no `argv`)? Is any command line parsed
  in one place, with one error policy?
- **Termination**: every `exit()`, `abort()`, and `assert()` in the
  library; a library should report and let the application decide.
- **Inheritance**: inheritance used only to reuse members (prefer
  composition), templates whose parameter has no effect, and derived
  classes that bypass their base. `protected` members and `virtual`
  functions only in classes designed for derivation; the others are
  `final`.
- **Symmetry**: the same concept offered in several forms with
  different signatures or capabilities; each form should offer the
  same features, unless the omission is deliberate and visible from
  the code.
- **Hidden policies**: execution order, deferred versus immediate
  execution, sorting, and anything else that the names do not reveal.
- **Dead or misplaced code**: public helpers used only by the tests or
  not at all, value types in a traits namespace, implementation
  helpers in public namespaces.
- **File layout**: declarations in `include/`, inline and template
  definitions in `include/.../inlines/<name>-inlines.h`, out-of-line
  definitions in `src/<name>.cpp`, one file per header of the same
  name; every source file registered in `xcdl-package.jsonc`, and the
  top-level `CMakeLists.txt` and `meson.build` matching the output of
  `xpm run xcdl-export` (they are never edited by hand).

### 4. Naming and consistency

- **Case**: identifiers use `snake_case` (types, functions, variables,
  members); file names are lower-case words separated by dashes (for
  example `<name>-inlines.h`).
- **Accessors**: getters and setters are nouns and share the name
  (`name()` and `name (const char* s)`), not `get_name()` and
  `set_name()`. Getters have no side effects; setters return `void`
  (no chaining) and change only the value they set.
- **Other member functions** start with a verb that states their
  effect. Flag names that do not describe the effect (a query that
  runs something, a `flush()` that does not flush the buffer).
- Member functions named after types, which force elaborated type
  specifiers (`class suite&`, `enum verbosity`).
- Several names for one concept, or several spellings for one entity.
  When there are many, add a consistency table to the report.
- Naming conventions with several meanings (the trailing underscore
  for non-public members).
- Deprecated entities without `[[deprecated]]`.
- `(void)` versus `()`, `virtual` repeated on overrides, `final` on
  members of `final` classes.
- Boolean and adjacent same-type parameters that are unreadable at
  the call site.
- Deviations from the patterns used elsewhere in the code base, without
  a visible reason.
- **Macros**: compiler-specific macros where a standard feature-test
  macro exists (`__EXCEPTIONS` instead of `__cpp_exceptions`,
  `__GXX_RTTI` instead of `__cpp_rtti`).
- **Header guards**: the path relative to `include/`, in upper case,
  with each non-alphanumeric character replaced by an underscore, and
  a trailing underscore (for example `MICRO_OS_PLUS_<NAME>_INLINES_<FILE>_H_`);
  unique across the project.
- **Layout** (checked on the original files): the formatting matches
  `maintenance/config/.clang-format` (the command above); no line
  exceeds 80 characters, except link URLs and the file path in a
  `DO NOT EDIT` comment of a generated file.

### 5. Simplicity and duplication

- Favour readability over cleverness and compactness.
- The same logic implemented twice or more; hand-written algorithms
  where the standard library has one.
- Families of near-identical templates that one parameterised
  template would replace.
- Unneeded indirection (pointers to vectors, `std::bind`, two-level
  hierarchies with nothing in one level).
- Unnecessary copies (operands stored by value and returned by value
  several times).

### 6. Modern C++

The code base targets C++20; flag code written in an older style where
a modern equivalent is clearly preferable:

- `concepts` and `requires` clauses instead of SFINAE and
  `std::enable_if`;
- `constexpr` and `consteval` instead of macros and run-time checks,
  where the value is known at compile time;
- range-based `for`, structured bindings, `auto`, and `<=>`, where
  they simplify the code without harming readability;
- `[[nodiscard]]`, `[[maybe_unused]]`, and the other standard
  attributes instead of compiler-specific equivalents;
- missing `const` on parameters, members, and member functions;
- missing or incorrect `noexcept` (move operations, destructors, and
  functions that cannot throw);
- raw pointers, casts, and macros where a standard facility would be
  clearer and safer, within the constraints of section 7.

### 7. Suitability for embedded targets

Program size matters more than speed. Unless `project.md` says
otherwise, the library may use the heap and the standard containers,
but it must build without exceptions and without RTTI.

- Heap allocations per operation; objects kept alive after use;
  `std::function` and `std::bind` overheads.
- Work done for output that is never printed.
- Fixed-size buffers and silent truncation.
- Dependencies on the hosted environment (`argv`, files, `exit()`,
  clocks), and whether the build works with `-fno-exceptions` and
  `-fno-rtti`.
- The stack frames reported by the cross build (`-fstack-usage`,
  `-Wframe-larger-than=512`).

Claims about the program size are measured, not estimated (with the
method given in `project.md` in this folder, if any, or by comparing
the sizes of the cross-compiled objects before and after the change);
quote the difference in the finding.

### 8. Incidental observations

Problems noticed along the way that are not API design: stale forward
declarations, macros leaking into user code, casts, fragile
compiler-specific code.

### 9. Reviewed and found sound

List the parts that were checked and are good, so that the maintainer
knows what was covered, and so that the report is not read as a list
of complaints only.

## Judging Findings

Follow "Judging Findings" in `.github/skills/review-common.md`. Cite the
line numbers from the stripped output, which keeps the original
numbers; verify every line number cited in a file outside the stripped
set (for example in `tests/`) before writing it.

## Numbering

Follow "Numbering" in `.github/skills/review-common.md`.

## Report

Write the report to a new file, `maintenance/docs/DESIGN-REVIEW-<n>.md`
(see "Report Files" in `.github/skills/review-common.md`; list the file
names only, do not open them):

```markdown
# Design review

Date: <date>, on branch `<branch>`, commit `<hash>`.

Scope: <files reviewed; that the Doxygen comments were ignored>.

Method: <how the code was read, the number of files and lines of code,
the compiler and flags used for the probes, the sanitizers, and who
wrote the newcomer programs (a sub-agent, or the reviewer before
reading the code)>.

Independence: the earlier reports in `maintenance/docs/`, and the
commits and issues that refer to them, were deliberately not
consulted.

Summary: <n> problems (<open> open, <fixed> fixed, <withdrawn>
withdrawn). <The most serious problems, by number, and the suggested
order of work: defects first, then non-breaking clean-ups, then the
breaking changes, grouped for one major release.>

<the meaning of the tags [breaking], [internal], and [confirmed]>

## 1. Defects

- **#1 — <short title>.** [breaking|internal] **[confirmed]**
  `<file>` lines <n> to <m>: <what is wrong, the evidence, and the
  suggested change>

## 2. Usability

## 3. Structure and layering

## 4. Naming and consistency

## 5. Simplicity and duplication

## 6. Modern C++

## 7. Suitability for embedded targets

## 8. Incidental observations

## 9. Reviewed and found sound

- <parts checked without problems>
```

Paths in the report are relative to the repository root, or shorter
when "Paths" in `.github/skills/project.md` allows it; `src/` paths are
always written in full.

Clearly mark the items related to deprecated functionality with the
tag `[deprecated]`, and group them at the end of their section.

## Procedure

1. Determine the scope, and the number of the new report (from the
   file names only; do not read the earlier reports).
2. Do the newcomer exercise, or start the sub-agent that does it,
   before reading anything else (see "Using the API as a Newcomer").
3. Strip the comments, check the headers, and check the formatting
   (commands above).
4. Read every stripped file in the order given, noting suspected
   problems with their file and line.
5. Read the test programs in `tests/` to see the intended use and the
   existing coverage.
6. Write probes for every suspected defect and every "does this
   compile/link" question; record the outcome.
7. Classify, tag, and number the findings, including those of the
   newcomer exercise; check them against the restrictions in
   `project.md` in this folder.
8. Write the report to the new file, and check the numbering.
9. Give the user a short summary: the counts, the most serious
   problems by number, and the decisions needed. Do not change the code
   unless asked; when the user later refers to a number, follow "Acting
   on an Item" in `.github/skills/review-common.md`.
