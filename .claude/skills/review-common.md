# Rules Shared by the Review Skills — µOS++ IIIe

<!--
DO NOT EDIT! Automatically generated from template file:
npm-packages-helper/templates/common/.github/skills/review-common-liquid.md;
put the project rules in .github/skills/project.md;
see .github/skills/README.md.
-->

This file is read by the review skills:

- `code-review`;
- `consistency-check`;
- `design-review`;
- `doxygen-review`;

and by the `github-commit` and `create-github-issue` skills where they
refer to review reports. It is not a skill.

The facts that are specific to this project (the tests to run, how to
build a probe, the public contract, the documentation to update) are
in `.github/skills/project-defaults.md` (the defaults) and in
`.github/skills/project.md` (the differences of the project), under
the headings named below; read them first. A heading that is absent
from `project.md` keeps the default. Where `project.md` disagrees with
this file, it takes precedence.

## Judging Findings

- Report a problem only when it can mislead a user, cause a wrong
  result, cost resources, open a security hole, or make the code harder
  to understand or maintain. Do not report a preference.
- Each finding cites the file and the line, states the problem
  directly, gives the evidence (the probe and its output, the
  reproduction steps, or the code path), and proposes a concrete
  change. When several solutions are reasonable, offer them and leave
  the decision to the maintainer.
- Verify every line number before writing it.
- Do not soften findings; state flaws directly.
- Tag each finding:
  - **[breaking]** when the suggested change alters the contract with
    the users, as listed under "Breaking Changes" in
    `.github/skills/project.md`;
  - **[internal]** when it does not;
  - **[confirmed]** when a probe or a reproduction confirmed it.
- For each **[breaking]** finding, state that it requires a major
  version (semantic versioning), and that the release notes must mark
  it as a breaking change. **The previous feature must be preserved**,
  marked as deprecated with
  `[[deprecated]]` (for example an alias or an overload of the new
  entity), and kept working for at least one major version; propose
  this migration path. Only when preserving the previous feature is not
  possible (for example, a changed result or error that cannot coexist
  with the old one, or a stricter type), give the reason why. In both
  cases give the text that the release notes should give to the users.

## Confirming Findings with Probes

A suspected defect is reported as a defect only after it has been
reproduced; otherwise say that it is derived from reading the code.

Write small probe programs in the scratchpad folder, never in the
repository, and build and run them as described under "Probes" in
`.github/skills/project.md`. Use one probe per question. Do not edit
the sources to build a probe.

Probes stay in the scratchpad; they are evidence for the report, not
tests. When a defect is fixed later, its probe becomes the regression
test (see "Acting on an Item").

## Numbering

Every actionable item in a report gets a permanent number, `#1`, `#2`,
and so on, counting through the whole report (not restarting in each
section), so that the user can refer to any of them (for example
"fix #12", or "apply #40 to #43"). The descriptive parts of the report
(the header, the summary, the notes on generated files) and the lists
of what was found sound or correct are not numbered. Each new report
starts again from `#1`, so an item is identified by the report and the
number (for example `DESIGN-REVIEW-3.md #12`).

- Each problem is a separate item; several occurrences of the same
  problem (for example the same typo in five places) are one item. Do
  not group independent problems under one number; a sub-list is
  acceptable only for the details of a single problem.
- The summary and the suggested order of work refer to the items by
  number, not by section.
- Numbers never change once assigned, and are never reused, not even
  for a withdrawn item.
- An item that is fixed later is marked `(fixed)`, with a one-line note
  and the commit, as described under "Review Documents" in
  `.github/skills/github-commit/SKILL.md`.
- An item that turns out to be wrong is marked `(withdrawn)` with the
  reason; it is not deleted.
- Write every item in the form `- **#<n> — <short title>.** ...`, or as
  a table row that starts with `| #<n> |`. In Markdown, a wrapped line
  must not start with `#` followed by a number (it becomes a heading);
  rewrap the paragraph.
- Check the numbering before finishing (set `f` to the report):

  ```sh
  f=maintenance/docs/<REPORT>.md
  { grep -oE '^- \*\*#[0-9]+' $f | grep -oE '[0-9]+'
    grep -oE '^\| #[0-9]+' $f | grep -oE '[0-9]+'; } | sort -n |
    awk '$1 != NR { print "mismatch at", NR, "found", $1; exit }
         END { print NR, "items" }'
  ```

## Naming GitHub Issues

A `#N` can be an item of a review report or a GitHub issue. Whenever a
GitHub issue is mentioned, in a reply, a question, a draft, or a
report, name it explicitly as a **GitHub issue** (for example "GitHub
issue #46", "create the GitHub issue", "the closing comment on the
GitHub issue"), never only as "the issue" or "issue #46", and call the
numbered item of a report an **item** (for example "item #1"). The
exceptions are the text that is posted on GitHub itself (the title and
the body of a GitHub issue, and its comments), where "issue" is
unambiguous, and the line `GitHub issue #N.` of a report item, which
has a fixed format.

## Report Files

Reports are written in the `maintenance/docs/` folder (create it if it
does not exist; it must be excluded from the npm package by
`.npmignore`; if it is not, tell the user), in British English and a
formal tone (no contractions, the Oxford comma, "folder" rather than
"directory"), with lines shorter than 80 characters, except inside
code blocks and tables when unavoidable. Paths in a report are relative
to the repository root, unless "Paths" in `.github/skills/project.md`
allows a shorter form.

| Skill                | Report file                                   |
| -------------------- | --------------------------------------------- |
| `code-review`        | `maintenance/docs/CODE-REVIEW-<n>.md`         |
| `consistency-check`  | `maintenance/docs/CONSISTENCY-CHECK-<n>.md`   |
| `design-review`      | `maintenance/docs/DESIGN-REVIEW-<n>.md`       |

`<n>` is one more than the highest number among the existing files of
the same kind (list the file names only; `1` if there are none). Never
overwrite an existing numbered report. "Review Documents" in
`.github/skills/project.md` may list older report names that still
count (for example after a skill was renamed); an unnumbered report
(for example `CONSISTENCY-CHECK.md`, from when the consistency check
kept a single file) counts as an earlier report of its kind.

The header of a report starts with a line
`Date: <date and time>, on branch <branch>, commit <hash>.`, with the
local time at which it was run (`date '+%Y-%m-%d %H:%M'`), the branch,
and the short commit (`git rev-parse --short HEAD`). The date is never
changed afterwards, not even when items are marked `(fixed)` or
`(withdrawn)`, because it decides which report is the most recent (see
below).

## Acting on an Item

When the user refers to a number (for example "fix #7"), it refers to
the most recent report, unless the user names another report. The most
recent report is the one with the latest `Date:` line among the files
in `maintenance/docs/`; the modification times of the files do not
count, since marking an item changes them.

**One item at a time.** Work on a single item, from the GitHub issue to
the commit and the report, and ask the user only about that item: do
not put questions, decisions, or approvals about several items in the
same message. Finish the item first, then propose the next one, naming
only that one.

1. Make sure that the previous fix is committed; do not start a new
   fix on top of uncommitted changes.
2. If the item has no GitHub issue, offer to create one with the
   `create-github-issue` skill, since the commit subject refers to it,
   and record its number in the report as described there.
3. Change the code, following the layout rules of the project.
   Keep the declarations in the headers, the inline and template
   definitions in `inlines/`, and the other definitions in `src/`.
   For a **[breaking]** item, preserve the previous feature, marked as
   deprecated (see "Judging Findings"), unless the item explains why
   that is not possible.
4. Keep the tests consistent with the change: a defect fix needs a new
   test that fails without the fix (usually the probe of the item); new
   functionality needs tests that cover it.
5. Update the documentation comments of every affected entity, and the
   hand-written documentation, as described under "Documentation
   Updates" in `.github/skills/project.md`.
   Follow the rules of the `doxygen-review` skill for the comments.
6. Run the tests listed under "Tests" in `.github/skills/project.md`,
   run the probe again, and report the outcome.
7. Commit only when the user asks, following the `github-commit` skill.
   Do not stage the report in the commit of the fix.
8. After the commit, mark the item `(fixed)` in the report that lists
   it, as described under "Review Documents" in the `github-commit`
   skill. Leave the report uncommitted; the user commits it separately,
   when they ask (after one fix, or once after several).
