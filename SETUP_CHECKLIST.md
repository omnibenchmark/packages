# Setup Checklist for Lab Conda Recipes

Quick checklist to get your repository up and running.

## ☐ Step 1: Create GitHub Repository

```bash
cd packages
git init
git add .
git commit -m "Initial commit: Lab conda recipes"
git remote add origin https://github.com/YOUR_USERNAME/YOUR_REPO.git
git branch -M main
git push -u origin main
```

## ☐ Step 2: Configure Trusted Publisher on prefix.dev

1. ✅ Log in to https://prefix.dev
2. ✅ Navigate to `almost-conductor` channel
3. ✅ Go to Settings > Trusted Publishers
4. ✅ Click "Add Trusted Publisher"
5. ✅ Configure:
   - Provider: **GitHub**
   - Repository owner: **your-github-username**
   - Repository name: **your-repo-name**
   - Workflow: **`.github/workflows/build.yml`**
6. ✅ Save configuration

**No API keys or secrets needed!** 🔐

## ☐ Step 3: Test the Workflow

```bash
# Make a test change
cd recipes/r-anndata
# Edit recipe.yaml: change build number from 0 to 1

git add recipe.yaml
git commit -m "Test CI: bump build number"
git push
```

Then check GitHub Actions tab to verify:
- ✅ Build succeeds
- ✅ Tests pass
- ✅ Upload to prefix.dev works

## ☐ Step 4: Install a Package

```bash
# Install from your channel
pixi add -c https://prefix.dev/almost-conductor r-anndata

# Verify it works
pixi shell
R -e "library('anndataR'); packageVersion('anndataR')"
```

## ☐ Step 5: Share with Lab

Share the installation instructions:

```bash
# Add channel
pixi project channel add https://prefix.dev/almost-conductor

# Install packages
pixi add r-anndata
```

---

## Troubleshooting

**Upload fails?**
- Verify trusted publisher configuration matches exactly
- Check repository name/owner spelling
- Ensure workflow path is correct

**Build fails?**
- Review build logs in GitHub Actions
- Test locally with `rattler-build build --recipe recipes/PKG/recipe.yaml`

**Package not found?**
- Wait a few minutes for indexing
- Check prefix.dev channel page
- Verify upload step succeeded in workflow logs

---

## Next Steps

📖 Read [CONTRIBUTING.md](CONTRIBUTING.md) to start adding recipes
🚀 Check [docs/GITHUB_SETUP.md](docs/GITHUB_SETUP.md) for advanced configuration
