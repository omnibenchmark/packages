# Lab Conda Recipes

Custom conda recipes for lab-maintained software and benchmark dependencies, built with
[rattler-build](https://github.com/prefix-dev/rattler-build) and published to the
`almost-conductor` channel on [prefix.dev](https://prefix.dev).

Recipes live here when the package isn't on conda-forge, needs a lab-specific patch, or
is still experimental.

## Using packages from our channel

```bash
# pixi
pixi project channel add https://prefix.dev/almost-conductor
pixi add r-anndata

# conda / mamba
conda install -c https://prefix.dev/almost-conductor r-anndata
```

Builds target `linux-64`, `osx-64`, `osx-arm64`, and `win-64`. Pure Python/R recipes are
built `noarch`. See [docs/PLATFORM_SUPPORT.md](docs/PLATFORM_SUPPORT.md) for details.

## Adding or updating a recipe

Full guide, recipe templates, common patterns (Python/Rust/CMake/Bioconductor), and
recipe-level troubleshooting are in [CONTRIBUTING.md](CONTRIBUTING.md).

Quick path for a package available on PyPI / CRAN / CPAN / LuaRocks — `rattler-build`
can scaffold the `recipe.yaml` for you:

```bash
mkdir -p recipes/r-fclust
rattler-build generate-recipe cran fclust > recipes/r-fclust/recipe.yaml
rattler-build build --recipe recipes/r-fclust/recipe.yaml
```

Review `requirements`, `license_file`, and `tests` before submitting — the generator
gives you a starting point, not a finished recipe.

## CI / publishing

GitHub Actions validates recipes on PRs and publishes to `almost-conductor` on merge to
`main`. Auth uses prefix.dev **Trusted Publishers** (OIDC) — no API keys.

The one-time admin setup (configuring the trusted publisher, repo wiring) lives in
[docs/GITHUB_SETUP.md](docs/GITHUB_SETUP.md); [SETUP_CHECKLIST.md](SETUP_CHECKLIST.md)
is the quick start.

For local manual uploads (rare): `export PREFIX_API_KEY=...` then
`rattler-build upload prefix -c almost-conductor output/<...>.conda`.

## Troubleshooting

For build/recipe issues (SHA mismatch, missing deps, failed tests, etc.) see the
troubleshooting section in [CONTRIBUTING.md](CONTRIBUTING.md#troubleshooting).

**Package built and uploaded but not installable / not showing up.** prefix.dev
re-indexes asynchronously after upload, so a freshly published package may not be
resolvable for a few minutes — occasionally longer under load.

1. Confirm the upload step succeeded in the Actions logs and the artifact appears on
   `https://prefix.dev/channels/almost-conductor`.
2. If it's listed but `pixi add` / `conda install` can't find it:
   - Wait 5–15 minutes and retry — indexing lag is the most common cause.
   - Clear local solver caches: `pixi clean cache` and/or `rm -rf ~/.cache/rattler`.
     Stale `repodata.json` will hide the new build.
   - Force a repodata refresh: `pixi update`, or pass `--no-cache` to `conda`/`mamba`.
3. Still missing after ~30 minutes — re-run the workflow; a partial upload may have
   left the channel without a valid `.conda` for that platform.
4. If you rebuilt with the same `version` and `build.number`, the channel may keep the
   older artifact. Bump `build.number` and re-publish.

## Resources

- [rattler-build docs](https://prefix-dev.github.io/rattler-build/) · [recipe format](https://prefix-dev.github.io/rattler-build/latest/reference/recipe_file/)
- [prefix.dev docs](https://prefix.dev/docs)
- [conda-forge recipes](https://github.com/conda-forge) — examples

## Maintainers

Maintained by the lab team. Open an issue, or come to the coffee-and-code meetup.
Individual recipes may carry their own licenses (see `about.license` in each
`recipe.yaml`).
