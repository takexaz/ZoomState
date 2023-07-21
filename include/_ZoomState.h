struct ST_ZOOM {
    EVAL_EXP x, y;
    EVAL_EXP scale;
};

struct ALLEG_BITMAP {
    int w, h;
    int clip;
    int cl, cr, ct, cb;
    unsigned char* line;
};

static auto SpriteArrayAccess = reinterpret_cast<DWORD * (*)(DWORD ptr, DWORD ptr2)>(0x44eeb0);
static auto DrawToScreen = reinterpret_cast<void (*)(void)>(0x4174e0);

static HINSTANCE allegro = GetModuleHandleA((LPCSTR)"alleg40");
static auto create_bitmap = reinterpret_cast<ALLEG_BITMAP * (*)(int width, int height)>(GetProcAddress(allegro, "create_bitmap"));
static auto destroy_bitmap = reinterpret_cast<void (*)(ALLEG_BITMAP * bitmap)>(GetProcAddress(allegro, "destroy_bitmap"));
static auto stretch_blit = reinterpret_cast<void (*)(ALLEG_BITMAP * source, ALLEG_BITMAP * dest, int source_x, int source_y, int source_width, int source_height, int dest_x, int dest_y, int dest_w, int dest_h)>(GetProcAddress(allegro, "stretch_blit"));
static auto blit = reinterpret_cast<void (*)(ALLEG_BITMAP * source, ALLEG_BITMAP * dest, int source_x, int source_y, int dest_x, int dest_y, int width, int height)>(GetProcAddress(allegro, "blit"));

int zoomReg(TPFILE*, STATE_INFO*, PLAYER_CACHE*);
void zoomProc(PLAYER*, STATE_INFO*);
void zoomFree(STATE_INFO*);
void zoomScreen(ALLEG_BITMAP*, int, int, float);
void zoomHook(void);
