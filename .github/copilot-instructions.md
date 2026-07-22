# TinyGSM Copilot Instructions

## Primary Workflow for Findings

When implementing a reported finding:

1. Verify the finding against the current code before editing.
2. Fix only findings that are still valid.
3. If a finding is no longer valid, skip it and include a brief reason.
4. Keep changes minimal and tightly scoped to the requested lines/files.
5. Validate after edits (at least file diagnostics; run targeted build/tests when practical).

## Editing Rules

- Do not refactor unrelated code while addressing a scoped finding.
- Preserve existing behavior unless the requested fix explicitly changes behavior.
- Preserve existing formatting style in touched files.
- Avoid broad mechanical changes unless explicitly requested.

## TinyGSM C++ and Modem Conventions

- Prefer existing TinyGSM patterns over introducing new abstractions.
- For modem string constants declared in PROGMEM, use flash-safe access helpers (for example GFP/GF patterns already used in the repo) rather than direct unsafe reads when String operations are involved.
- Reuse existing modem trait/config values (for example ModemConfig::\* constants) instead of adding duplicate literals.
- Maintain cross-modem consistency: when changing one modem implementation, check equivalent implementations for intended parity, but edit only what is requested.

## Safety and Validation Expectations

- Never revert or rewrite unrelated local changes.
- Prefer smallest possible patch to satisfy the request.
- After each change, report:
  - what was changed,
  - what was intentionally skipped (if anything),
  - what validation was performed.

## Suggested Response Style

For scoped fix requests, respond in this order:

1. Findings addressed (or skipped with reason).
2. Exact files changed.
3. Validation results.
