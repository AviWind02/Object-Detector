namespace core {
    namespace window {
        HWND Create(int x, int y, int width, int height);
        void Show(HWND hwnd);
        void Destroy(HWND hwnd);
        void MessageLoop(HWND hwnd);
        extern ImVec2 windowPos;
        extern ImVec2 windowSize;
    }
}