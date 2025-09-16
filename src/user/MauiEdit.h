#include "user/MauiControl.h"
#include "user/MauiDragger.h"
#include "core/D3DRes.h"
#include "core/Win.h"
#include "gpgcore/String.h"

namespace Moho {

class CMauiEdit :
    public Moho::CMauiControl, // 0x00E39564
    public Moho::IMauiDragger  // 0x00E395CC
{
public:
    Moho::CountedPtr<Moho::CD3DFont> mFont;
    unsigned int mForegroundColor;
    bool mBackgroundVisible;
    unsigned int mBackgroundColor;
    unsigned int mHighlightForegroundColor;
    unsigned int mHighlightBackgroundColor;
    bool mDropShadow;
    bool mIsEnabled;
    std::string mText;
    unsigned int mCaretPosition;
    bool mCaretVisible;
    unsigned int mCaretColor;
    unsigned int mCaretCurColor;
    float mCaretCycleSeconds;
    unsigned int mCaretCycleOnColor;
    unsigned int mCaretCycleOffColor;
    float mCaretTime;
    int mClipOffset;
    int mClipLength;
    int mSelectionStart;
    int mSelectionEnd;
    int mDragStart;
    bool mDoingCallback;
    int mMaxChars;
    int v29;

    static gpg::RType *StaticGetClass(); // 0x0078EC60
    gpg::RType *GetClass() const override; // 0x0078EC80
    gpg::RRef GetDerivedObjectRef() override; // 0x0078ECA0
    ~CMauiEdit() override = default; // 0x0078F190
    void DoRender(Moho::CD3DPrimBatcher *batcher, unsigned int renderPass) override; // 0x0078F820
    bool HandleEvent(const Moho::SMauiEventData &ev) override; // 0x00790470
    void Frame(float delta) override; // 0x0078F720
    void AbandonKeyboardFocus() override; // 0x0078F330
    void LosingKeyboardFocus() override; // 0x007915A0
    void Dump() override; // 0x0078F280
    //~CMauiEdit(IMauiDragger) override; // 0x00795A00 -> 0x0078F190
    void DragMove(Moho::SMauiEventData *ev) override; // 0x007913A0
    void DragRelease(Moho::SMauiEventData *ev) override; // 0x007914C0
    void DragCancel() override {} // 0x00791590

    void SetText(std::string *text); // 0x0078F380
    void ClearText(); // 0x0078F4C0
    void SetMaxChars(int max); // 0x0078F570
    bool EnterPressed(); // 0x007904D0
    bool EscPressed(); // 0x007904F0
    void NonTextKeyPressed(int key, const Moho::SMauiEventData &ev); // 0x00790510
    void DeleteCharAtCaret(bool toRight); // 0x00790B40
    std::string GetSelection(); // 0x007906F0
    void ClearSelection(); // 0x007907B0
    void DeleteSelection(bool supress); // 0x00790590
    void ReplaceSelection(const std::string &str); // 0x00790830
    void SetClipOffsetRight(int pos); // 0x00790D20
    void SetClipOffsetLeft(int pos); // 0x00790F90
    void SetCaretPosition(int pos); // 0x007911B0
    void MoveCaretLeft(int amt); // 0x00791250
    void MoveCaretRight(int amt); // 0x00791270
    void MoveSelectionLeft(int amt); // 0x00791290
    void MoveSelectionRight(int amt); // 0x00791310
    void HandleClickEvent(const Moho::SMauiEventData &ev); // 0x007915C0
    void HandleKeyEvent(const Moho::SMauiEventData &ev); // 0x00791780
    void TextChanged(std::string *newText, std::string *oldText); // 0x00794F20
    bool CharPressed(int *keycode); // 0x007953D0

    const char *GetText() const {
        return this->mText.c_str();
    }
    int GetTextLen() const {
        return gpg::STR_Utf8Len(this->GetText());
    }
    int GetClipThreshold(int pos) {
        const char *textBegin = this->GetText();
        int startPos = this->mText.size() - gpg::STR_Utf8ByteOffset(textBegin, pos);
        const char *thres = &textBegin[startPos];
        float totalAdv = 0.0;
        while (thres != textBegin) {
            thres = gpg::STR_PreviousUtf8Char(thres, textBegin);
            wchar_t wChar;
            gpg::STR_DecodeUtf8Char(thres, wChar);
            totalAdv += this->mFont->GetCharInfo(wChar).mAdvance;
            if (totalAdv > this->Width()) {
                thres = gpg::STR_DecodeUtf8Char(thres, wChar);
                break;
            }
        }

        int clipSize = startPos - (thres - textBegin) + 1;
        std::auto_ptr<char> testClipLen{new char[clipSize]};
        memcpy(testClipLen.get(), thres, clipSize - 1);
        testClipLen.get()[clipSize - 1] = '\0';
        return gpg::STR_Utf8Len(testClipLen.get());
    }
    void CopyToClipboard() {
        std::string select = this->GetSelection();
        std::wstring wSel = gpg::STR_Utf8ToWide(select.c_str());
        Moho::WIN_CopyToClipboard(wSel.c_str());
    }
};

}
