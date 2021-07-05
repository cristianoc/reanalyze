# Development

## Build reanalyze

```
# Installs all dependencies (if needed) and builds reanalyze
npm install
dune build
```

## Test reanalyze

Make sure to always run the tests before submitting any changes (CI usually takes
longer to give you feedback).

```
npm test
```

## Releases (for Maintainers)

The project is compatible with the [`npm
version`](https://docs.npmjs.com/cli/version) workflow. After using the `npm
version [major|minor|patch|...]` command, npm will automatically tag the
current commit, bump all the necessary version numbers (also the number in
`src/Version.ml`) and push it to the current remote branch.

Use the `...` menu next to the [tag](https://github.com/rescript-association/reanalyze/tags) to create a release manually. (To be automated later).

**Here are the concrete commands to run:**

```
# Make sure to commit & push all current changes, the working branch should be clean
# and synced up with your remote branch

# Also make sure that your current branch is explicitly set to the relevant remote
# (`git push` instead of `git push origin master`)
git status

# For patches (0.0.X+1)
npm version patch

# For minor (0.X+1.0)
npm version minor

# For major (X+1.0.0)
npm version major
```

## Releasing to npm (Maintainers only)

Use the GitHub web iterface to download the artifact `reanalyze-npm.tar`.

```
# Dry run for testing
npm publish reanalyze-npm.tar --dry-run

# Publish package as @latest
npm publish reanalyze-npm.tar

# Publish package with @beta tag
npm publish reanalyze-npm.tar --tag beta
```

Consult the [npm publish](https://docs.npmjs.com/cli/publish) documentation for more options.
In case you get an `ENEEDAUTH` error, use `npm adduser` and authenticate with your npm account first.
