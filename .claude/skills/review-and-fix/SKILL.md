---
name: review-and-fix
description: The µOS++ IIIe workflow for fixing the items of a review report one at a time - re-derive each fix against the current code and both ports, propose it, apply it with a deterministic RTOS test and a negative check, run the native and QEMU suites, then hand over to the create-github-issue and github-commit skills, keeping the report up to date. Use it whenever the user works through review findings in this project ("how do you suggest to fix #N", "apply it", "create github issue for #N and commit", "commit without a github issue", "check the same issue in the other packages", "anything else left?"), even if the word "skill" is not used.
---

# µOS++ IIIe review and fix workflow

This skill belongs to the project (it is not generated). It holds the
parts of the review and fix work that are specific to µOS++ IIIe: the
ports, the RTOS test patterns, the negative checks, and the binary
comparisons. The common rules are in the generated skills, which it
follows:

- `.github/skills/review-common.md` - the findings, the numbering, the
  report files, and "Acting on an Item";
- `code-review` - writing the report;
- `create-github-issue` and `github-commit` - the GitHub issues, the
  commits, and the issue comments;
- `.github/skills/project.md` and the `project.md` of each skill
  folder - the facts of this project (the tests, the probes, the
  ports, the review documents).

Read `CLAUDE.md` first (build, test, formatting, port contract, coding
conventions, including the portability rule). For the details that are
not needed every time (test patterns, negative checks, pitfalls), read
`references/details.md` when you reach that step.

The user drives the pace; each step ends by reporting and, when a
decision is needed, asking.

## Phase 1 - the review report

Write the report with the `code-review` skill (its RTOS-specific
checks are in `code-review/project.md`), to the next
`maintenance/docs/CODE-REVIEW-<n>.md`. Agree the scope first (for
example `git diff xpack...xpack-development`, a pull request, or a
folder). Look for real bugs added or made worse by the change: the
scheduler and thread state, the synchronisation objects, the startup,
the C API, and the ports; check every claim against the code, both
ports, the documentation, and the tests. Suggest a fix order in phases
(small and contained first, decisions next, the scheduler core last,
one at a time with tests in between).

`CODE-REVIEW-1.md` predates the skills and keeps its own format (see
"Review Documents" in `.github/skills/project.md`).

## Phase 2 - the fix loop

The user works through the items with a small set of prompts. Respond
to each as below, following "Acting on an Item" in
`.github/skills/review-common.md` (one item at a time).

### "how do you suggest to fix #N?"

Do not just repeat the suggestion of the report; re-derive it.

- Re-read the current code: earlier fixes may have changed it, and the
  report may be wrong, overstated, or understated (for example a window
  narrower than described, or the same bug in other objects). Say so
  explicitly; this is where most of the value is.
- Check both ports, the `smp` branch when relevant, and what the
  documentation promises.
- Give one recommendation (not a survey), with a short code sketch,
  and say what it does not change (ports, C API, data layout) or, if it
  does, why.
- Call out behaviour changes; they need a `CHANGELOG.md` entry
  (`**Behaviour change:** ...`) and the user's agreement.
- Propose the tests: deterministic where possible (thread priorities,
  the scheduler lock, a pending interrupt; see `references/details.md`),
  a stress test when a race cannot be forced, or regression-only tests
  when nothing can reach the case, saying so honestly. Plan a negative
  check.
- End with "Shall I apply it?". If the user adds constraints or corrects
  you, fold them in and propose again rather than arguing; if a choice
  is genuinely theirs (naming, layout, two valid designs), ask with
  concrete options.

### "yes, apply it"

1. Implement the fix. Keep the trace and instrumentation calls, the
   `in_handler_mode()` checks, British English, "folder" not
   "directory", the µOS++ licence header in new files, and the
   portability rule.
2. Add the tests to the `unit` test (`tests/sources/unit/src/`);
   `rtos-apis` only checks that the API calls are present.
3. **Negative check**: show that the new test fails without the fix
   (see `references/details.md`). If the test passes without the fix,
   the test is wrong (it happened: a waiter that never actually
   blocked); fix the test.
4. Format only the touched files:
   `xpacks/.bin/clang-format --style=file:maintenance/config/.clang-format -i <files>`
   (CMake files: `xpm run cmake-format`, or `cmake-format` with
   `maintenance/config/.cmake-format.py` on the touched files only).
5. Run the tests listed under "Tests" in `.github/skills/project.md`.
   Long runs may exceed the tool timeout; run them in the background.
6. For refactorings that must not change the behaviour, compare the
   release binaries before and after with `scripts/hash-builds.sh` (the
   CMSIS validator embeds `__TIME__`, so a few timestamp bytes differ
   there).
7. Update the report: mark the item as described under "Review
   Documents" in the `github-commit` skill, or, for `CODE-REVIEW-1.md`,
   in its own format (the `**Status:**` line, "Analysis before fixing"
   when the report was not exactly right, "Fix applied", and
   "Verification" with the negative check result).
8. Report concisely: what changed, the test results, what could not be
   verified, and the behaviour changes. Do not commit.

### "create github issue for #N and commit"

1. Create the GitHub issue with the `create-github-issue` skill (its
   draft is shown to the user before it is opened, unless the prompt
   already approves it), and record its number in the report.
2. Commit with the `github-commit` skill: only the files of this fix
   (sources, headers, tests, and `CHANGELOG.md` when changed), never the
   report, the user's own edits, or other fixes.
3. Never push unless asked; the issue comment of `github-commit` waits
   for the push.

### "commit without a github issue"

For small changes (typos, version bumps, test-only fixes,
documentation), follow "Commits without an Issue" in the
`github-commit` skill: the named files only.

### "check the same issue in the other packages / fix it there too"

Survey the sibling repositories (the ports, `xPacks/*`) for the same
pattern, report a table (package, code, result), and fix only where
asked. Other repositories have their own conventions (branch, commit
style, issue tracker; a fork may have issues disabled and no push
rights); check before committing, and report what cannot be pushed.

### "anything else left?"

Check `git status`, `git rev-list --count origin/<branch>..HEAD`, the
stashes, and the report, then list what is left, grouped: decisions
for the user (push, committing the report, other branches, release
steps) and things noticed but not part of the review. Offer the next
step instead of starting it.

## Things that went wrong before (avoid them)

- A fix proposal that copied the suggestion of the report without
  checking the code missed that the bug was elsewhere too, or narrower.
- A guard using `__ARM_EABI__` was rejected: the code must stay
  portable (see the portability rule in `CLAUDE.md`).
- Stashing for a baseline build swept in a file the user was editing at
  the same time; always stash with an explicit list of your own files.
- Linked dependencies (`xpm link`) can point at sibling repositories on
  the wrong branch (for example `smp`); when tests fail in CMake before
  compiling, check the links and the branches first.

More pitfalls (zsh quoting, compiler warnings, test design) are listed
in `references/details.md`.

## Scripts

- `scripts/build-probe.mjs` - builds and runs a probe against a test
  build folder (see "Probes" in `.github/skills/project.md`).
- `scripts/hash-builds.sh` - hashes the release test binaries, to show
  that a refactoring does not change the generated code.
