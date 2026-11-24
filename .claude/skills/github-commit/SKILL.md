---
name: github-commit
description: 'Prepare and create a git commit in the µOS++ IIIe repository (micro-os-plus/micro-os-plus-iii), following the project''s commit message conventions, and add the closing comment to the GitHub issue it fixes. Use when the user asks to "commit", "make a commit", "commit the fix", or to write a commit message for the current changes.'
argument-hint: 'Optional: the GitHub issue number (#N), and whether the commit completes the issue'
---

<!--
DO NOT EDIT! Automatically generated from template file:
npm-packages-helper/templates/common/.github/skills/github-commit/SKILL-liquid.md;
put the project rules in .github/skills/project.md (all the skills) or
in .github/skills/github-commit/project.md (this skill only);
see .github/skills/README.md.
-->

# Commit — µOS++ IIIe

## When to Use

- The user asks to commit the current changes, or a subset of them.
- The user asks for a commit message for the staged or unstaged
  changes.

This skill is the reference for the commit messages and the issue
updates; `.github/copilot-instructions.md` points to it, and requires
the user approval of the file list and the message before each commit.

Read `.github/skills/project-defaults.md`, `.github/skills/project.md` first, and `project.md` in this
folder, if it exists; their rules extend these, and take precedence
where they differ.

In the replies and the questions to the user, name the issues
explicitly as GitHub issues (see "Naming GitHub Issues" in
`.github/skills/review-common.md`).

## Procedure

1. **Inspect the working tree.** Run `git status` and `git diff` (and
   `git diff --cached` if anything is staged). Identify which changes
   belong to this commit; the working tree often contains unrelated
   changes and untracked files (for example the regenerated common
   files, the review reports and the notes in `maintenance/docs/`, or
   the files listed under "Files Not to Commit" in
   `.github/skills/project.md`), which must not be included unless the
   user says so.
2. **Identify the GitHub issue.** Each commit refers to a GitHub issue,
   the maintenance commits included; a commit has no issue only when the
   user explicitly decides so (see "Commits without an Issue"). If the
   user did not give the number and it cannot be determined with
   certainty from the conversation, ask for it; do not guess. If no
   issue exists, offer to create one with the `create-github-issue`
   skill. Ask
   whether this commit completes the issue (and gets `Fixes #N`), or is
   one of several commits for it, unless that is already clear. A
   commit may fix several issues; it then has one `Fixes #N` line for
   each, and the subject has the number of the main one.
3. **Check that the tests were run**, as listed under "Tests" in
   `.github/skills/project.md`. If they were not run after the last
   change, say so and run them, or ask the user, before committing.
   Changes limited to the website, to the documentation, or to the
   generated common files do not need the tests; say so. Report
   failures; do not commit over them without explicit approval.
4. **Check the generated files.** If the source of a generated file
   changed (see "Generated Files" in `.github/skills/project.md`), the
   generated files must be regenerated and included, not edited by
   hand. The reference outputs that the tests compare with (if the
   project has any) are updated in the same commit as the change that
   alters them.
5. **Draft the message** (see "Message Format") and write it to a file
   in the scratchpad folder.
6. **Show the user** the list of files to be committed and the full
   message, and wait for explicit approval. Never commit without it.
7. **Stage and commit** only the approved files, by name (no
   `git add -A` or `git add .`), and commit with the message file:

   ```sh
   git add <file> ...
   git commit -F <scratchpad>/commit-message.txt
   ```

   Do not amend, rebase, or skip the hooks (`--no-verify`) unless the
   user asks. If a hook fails, fix the cause and create the commit
   again. Do not push unless the user asks.
8. **Report** the commit hash and subject, with
   `git log -1 --format='%h %s'`. Use the short hash printed by `%h`
   everywhere (its length is set by the repository).
9. **Update the GitHub issue**, if the commit has `Fixes #N` (see
   "Issue Comment"). The comment is posted only after the commit is
   pushed; if it is not pushed yet, tell the user that the comment is
   pending, and post it when the user asks for the push (or says that
   it was pushed).
10. **Ask whether to update the review documents** that list the issue
    or the finding it came from (see "Review Documents").

Do not start the next fix while this one is uncommitted; commit first.

## Branches

Work is committed on `xpack-development`. `xpack`
receives merges at release time only; never commit directly to it. Do
not commit to the other branches of the repository unless the user
says so.

## Message Format

### Subject

The subjects follow the same rule in all the µOS++ and xPack
repositories, which is that of git itself (its
`Documentation/SubmittingPatches`): an optional prefix, then a summary
in lower case.

- The form is `[<prefix>: ]<summary>`, with at most one prefix:
  - `#N` for a change that refers to a GitHub issue
    (`#152: write the report lines to both streams once`);
  - `website` for a change limited to `website/`
    (`website: add the footprint page`; the routine update is
    `website: update`);
  - the name of a file, for a change limited to that file
    (`package.json: bump typescript`).
- The summary starts with a lower-case letter, unless its first word
  has a fixed case (`CHANGELOG`, `README`, `µTest++`, an identifier).
- It is in the imperative mood, and describes the change, not the
  problem (the issue title describes the problem):
  `retry the partial writes`, not `the partial writes are lost`.
- At most 50 characters, including the prefix.
- No period at the end, and no backticks.
- No type prefixes (`fix:`, `feat:`); the repositories do not use
  Conventional Commits.
- When the user decides that a maintenance commit has no GitHub issue
  (see "Commits without an Issue"), it uses the usual subject:
  `website: update`, `re-generate top commons`,
  `re-generate website commons`, `re-generate tests commons`,
  `dist update`, or `package.json update`; the release commits use
  the subjects of "Release Commits".

### Body

Optional for the trivial and the maintenance changes; required for a
fix, also in a repository whose earlier commits are mostly single
lines: those were written by hand, while the body of a commit written
by the agent explains what and why to the reviewer. Separated from the
subject by a blank line, wrapped at 72
characters, in British English, formal, with no contractions and with
the Oxford comma. Identifiers, files, and options are written plainly,
without backticks. Explain what and why, not how; usually three short
paragraphs:

1. **The previous state**, in the past tense: what the code did, and
   why it was a problem.

   > When the host accepted only part of the buffer, the write
   > returned the shorter count, and the remaining bytes of the line
   > were lost.

2. **The change**, in the imperative: what was added, removed, or
   changed, and the decisions that are not obvious from the diff
   (what was deliberately left unchanged, and why).

   > Repeat the call until all the bytes are written, and return the
   > number of bytes actually written.

3. **Tests and documentation**, in the imperative: the tests added or
   changed, the reference outputs, and the documentation updated (or
   the probe or the sample project that verified the change).

   > Add a test for the partial writes, and update the documentation
   > comments of write().

A trivial change may have a single paragraph.

Mention the related issues and pull requests, if any, by number, where
they are relevant in the text: an earlier decision or change that this
commit builds on or reverts (`as #61 chose`, `removed by #109`), or a
follow-up left to another issue (`see #51`). Refer to an item of a
review document by the document name and the item number, so that it
is not mistaken for an issue: `(DESIGN-REVIEW-2.md #43)`.

### Trailers

After the body, separated by a blank line:

- `Fixes #N` — only on the commit that completes the issue; omit it
  from the intermediate commits of an issue fixed in several commits.
- The attribution trailer required by the agent's environment, if any
  (for example `Co-Authored-By: ...`), copied exactly as given there,
  as the last line. The example below omits it, because it depends on
  the environment; it is not optional when the environment requires
  it.

### Example

A hypothetical example, which illustrates the format only; the project
may give real ones in `project.md` in this folder:

```text
#12: retry the partial writes in write()

When the host accepted only part of the buffer, write() returned the
shorter count, so the remaining bytes of the line were lost.

Repeat the call until all the bytes are written, and return the number
of bytes actually written.

Add a test for the partial writes, and update the documentation
comments of write().

Fixes #12
```

## Commits without an Issue

Every commit refers to a GitHub issue, unless the user explicitly
decides that a given change needs none (for example a documentation or
a website update, or another maintenance commit). Never decide it on
the user's behalf: when no issue is known, ask for the number, or offer
to create the issue. For a commit that the user decided has no issue:

- The subject has no `#N: ` prefix; the other subject rules apply.
- The message has no `Fixes` trailer, and there is no issue comment.
- Ask whether a review document lists the change (see "Review
  Documents"); it is then recorded with the commit hash only.

## Release Commits

The release commits are mechanical and are made by the maintainer
during a release, following the release procedure of the project (in
the maintainer pages of the website, or in
`maintenance/docs/README-MAINTAINER.md`). Do not create them, and do
not run `npm version`, whose `postversion` script pushes the branches
and the tags, unless the user is performing a release. Their subjects,
in order:

1. `CHANGELOG: prepare vX.Y.Z` — the `CHANGELOG.md` entry of the
   release;
2. `CHANGELOG: publish npm vX.Y.Z` — the `CHANGELOG.md` update for the
   publication on npm;
3. `X.Y.Z` — the version bump, made by `npm version`.

## Issue Comment

After a commit with `Fixes #N`, add a comment to the GitHub issue,
after the commit is pushed (GitHub links a hash only then, and an amend
or a rebase changes it):

```sh
gh issue comment <N> --repo micro-os-plus/micro-os-plus-iii \
  --body-file <scratchpad>/issue-comment.md
```

The comment is short: the commit message already explains the change
and how it was verified, and GitHub shows the commit under the issue.

- If the proposal of the issue was followed, the comment is a single
  line, `Fixed in <short hash>, on YYYY-MM-DD.` (with several hashes,
  `Fixed in <hash> and <hash>, on YYYY-MM-DD.`).
- Only if the proposal of the issue was not followed, or a part of it
  was deliberately not done, or the finding was withdrawn, use three
  paragraphs: the line `Fixed in <short hash>.`, then one or two
  sentences with what differs and why, then the separate line
  `Fixed on YYYY-MM-DD.`. Do not repeat the explanation of the change
  or of its verification.
- The date is that of the commit (`git log -1 --format=%cs`).
- Write the commit hashes bare, without backticks (`Fixed in 123f0c76`),
  so that GitHub links them; in backticks they are not linked. (The
  review documents are read as files in the repository, so they write
  the hashes in backticks; do not make the two consistent.)
- Write one line per paragraph, in Markdown, with backticks for
  identifiers (but not for the commit hashes); GitHub renders every
  newline in an issue comment as a line break, so the paragraphs are
  not wrapped.
- Do not close the issue; it is closed when the commit reaches the
  default branch (`xpack`).

For example, for an issue whose proposal was followed:

```markdown
Fixed in a8733dc2, on 2026-11-11.
```

Show the comment to the user before posting it, unless the user has
already approved posting it together with the commit; for several
issues, show them all. Posting a comment is a public action.

## Review Documents

The findings that lead to issues are often recorded in review, change,
or comparison documents, mainly in `maintenance/docs/` (the reports of
the review skills, listed under "Report Files" in
`.github/skills/review-common.md`, and the other documents listed under
"Review Documents" in `.github/skills/project.md`), and in other files
named `*-review*`, `*-changes*`, `*-compatibility*`, or similar, or in
a document that the user mentions (a notes file, for example). If there
are none, skip this section. After the commit, search them for
the issue and the finding. When the finding came from a known document
(for example, the issue was created from `DESIGN-REVIEW-8.md`), search
that document first, by the words of the issue title and by the item
number; otherwise search all the documents:

```sh
grep -n -i -E '<words from the issue title>|\(#<N>\)|issue #<N>' \
  maintenance/docs/*.md
```

Replace `<N>` with the number of the issue. In the reports, a `#K` at
the start of an item is the number of the finding within the report,
not a GitHub issue number; match the GitHub number where it is used as
such (in parentheses, or after "issue" or "fixed in"), and confirm the
item by its content.

When an issue is created from an item (see the `create-github-issue`
skill), its number is recorded as a line `GitHub issue #N.` at the end
of the item, which the search above finds:

```markdown
- **#7 — <short title>.** [internal] **[confirmed]**
  ...
  GitHub issue #61.
```

If an item refers to the issue or to the same finding, **ask the user**
whether to update it, and show the proposed text. Then add `(fixed)`
after the tags of the item (after the title, for the items that have no
tags), and a line at its end that mentions both the short commit hash
and the GitHub issue number:

```markdown
- **#7 — <short title>.** [internal] **[confirmed]** (fixed)
  ...
  GitHub issue #61.
  Fixed in `a8733dc2` (GitHub issue #61): <what was done>.
```

The other documents use the formats listed under "Review Documents" in
`.github/skills/project.md`; the older items of a report may use an
earlier format, which is kept as it is.

Summarise what was done, and state where the fix differs from the
recommendation of the document. For an intermediate commit of an issue
fixed in several commits, ask whether to record it as partial
(`Partly fixed in ... (GitHub issue #N): ...`). For a withdrawn finding,
write `Withdrawn (GitHub issue #N): <reason>.` instead, and add
`(withdrawn)` after the tags.

Do not change the `Date:` line of the document, which records when the
review was made, and decides which report is the most recent. These
documents are often untracked or have unrelated changes; do not stage
or commit them unless the user asks.
