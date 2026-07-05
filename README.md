# LibNXBox Account Switcher

A tiny homebrew app for Nintendo Switch that lets you swap between two
(or more) saved LibNXBox login tokens without manually renaming files
over FTP every time.

## How it works

LibNXBox reads whichever file is named `token.dat` in
`/switch/libnxbox/` when it starts. This app copies one of your saved
token files over `token.dat` for you, with a simple on-screen menu.

## One-time setup (do this once)

1. Launch LibNXBox and log in with your **Fortnite** account.
   This creates `/switch/libnxbox/token.dat`.
2. Copy that file (using any SD file manager, e.g. a homebrew file
   browser or your CFW's built-in one) to:
   `/switch/libnxbox/token_fortnite.dat`
3. In LibNXBox, log out and log back in with your **Game Pass**
   account. This overwrites `token.dat` again.
4. Copy the new file to:
   `/switch/libnxbox/token_gamepass.dat`

Now both saved logins exist side by side and neither will be lost.

## Everyday use

1. Launch **Account Switcher** from the Homebrew Menu.
2. Use Up/Down to highlight "Fortnite" or "Game Pass".
3. Press **A** to switch — this copies the matching file over
   `token.dat`.
4. Press **+** to exit, then launch **LibNXBox** as normal.

Each token is independent — switching only decides which one
`token.dat` currently is. Whichever account's token is active is the
one LibNXBox authenticates as, so a Fortnite-only account will only
have Fortnite access, and your main account keeps full Game Pass
access, automatically.

## Adding more profiles

Edit `source/main.cpp` and add more entries to the `profiles` list:

```cpp
static std::vector<Profile> profiles = {
    { "Fortnite",   "token_fortnite.dat" },
    { "Game Pass",  "token_gamepass.dat" },
    { "Third One",  "token_third.dat" },
};
```

Then push to GitHub — the Actions workflow will rebuild the `.nro`
automatically.

## Building

This repo builds automatically via GitHub Actions on every push to
`main`. Go to the **Actions** tab on GitHub, open the latest run, and
download the `token-switcher-nro` artifact — it contains
`token-switcher.nro`.

Copy that file to `/switch/token-switcher/token-switcher.nro` on your
SD card (create the folder if it doesn't exist), then it will show up
in the Homebrew Menu.
