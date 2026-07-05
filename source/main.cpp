// LibNXBox Account Switcher
// Simple homebrew app to swap token.dat between saved profiles
// (e.g. "Fortnite" account and "Game Pass" account)

#include <switch.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <sys/stat.h>

static const char* BASE_DIR   = "sdmc:/switch/libnxbox";
static const char* ACTIVE_TOKEN = "sdmc:/switch/libnxbox/token.dat";

struct Profile {
    std::string label;      // shown on screen
    std::string filename;   // token_xxx.dat
};

// Add or edit profiles here. filename must exist inside /switch/libnxbox/
static std::vector<Profile> profiles = {
    { "Fortnite",   "token_fortnite.dat" },
    { "Game Pass",  "token_gamepass.dat" },
};

static bool fileExists(const std::string& path) {
    struct stat buffer;
    return (stat(path.c_str(), &buffer) == 0);
}

static bool copyFile(const std::string& src, const std::string& dst) {
    FILE* in = fopen(src.c_str(), "rb");
    if (!in) return false;

    FILE* out = fopen(dst.c_str(), "wb");
    if (!out) {
        fclose(in);
        return false;
    }

    char buf[4096];
    size_t n;
    bool ok = true;
    while ((n = fread(buf, 1, sizeof(buf), in)) > 0) {
        if (fwrite(buf, 1, n, out) != n) {
            ok = false;
            break;
        }
    }

    fclose(in);
    fclose(out);
    return ok;
}

int main(int argc, char* argv[]) {
    consoleInit(NULL);

    // Basic controller input (button-based, works with touch-capable consoles too)
    PadState pad;
    padConfigureInput(1, HidNpadStyleSet_NpadStandard);
    padInitializeDefault(&pad);

    int selected = 0;
    bool running = true;
    std::string statusMsg = "";

    while (appletMainLoop() && running) {
        padUpdate(&pad);
        u64 kDown = padGetButtonsDown(&pad);

        if (kDown & HidNpadButton_Up) {
            selected = (selected == 0) ? (int)profiles.size() - 1 : selected - 1;
            statusMsg.clear();
        }
        if (kDown & HidNpadButton_Down) {
            selected = (selected + 1) % profiles.size();
            statusMsg.clear();
        }
        if (kDown & HidNpadButton_A) {
            const Profile& p = profiles[selected];
            std::string srcPath = std::string(BASE_DIR) + "/" + p.filename;

            if (!fileExists(srcPath)) {
                statusMsg = "Missing file: " + p.filename;
            } else if (copyFile(srcPath, ACTIVE_TOKEN)) {
                statusMsg = "Switched to: " + p.label + " - launch LibNXBox now";
            } else {
                statusMsg = "Error copying file. Check SD card.";
            }
        }
        if (kDown & HidNpadButton_Plus) {
            running = false;
        }

        consoleClear();
        printf("\x1b[1;1HLibNXBox Account Switcher\n");
        printf("\x1b[2;1H--------------------------------\n");
        printf("\x1b[3;1HUp/Down: choose   A: switch   +: exit\n\n");

        for (size_t i = 0; i < profiles.size(); i++) {
            if ((int)i == selected)
                printf("  > %s\n", profiles[i].label.c_str());
            else
                printf("    %s\n", profiles[i].label.c_str());
        }

        if (!statusMsg.empty()) {
            printf("\n%s\n", statusMsg.c_str());
        }

        consoleUpdate(NULL);
    }

    consoleExit(NULL);
    return 0;
}
