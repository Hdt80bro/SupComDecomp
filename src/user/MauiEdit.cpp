#include "MauiEdit.h"
#include <memory>

DEFINE_ROBJECT_COMMON(Moho::CMauiEdit)


// 0x0078F820
void Moho::CMauiEdit::Draw(Moho::CD3DPrimBatcher *batcher, int renderPass) {
    if (this->mFont == nullptr) {
        return;
    }

    float left = this->GetLeft();
    float right = this->GetRight();
    float top = this->GetTop();
    float bottom = this->GetBottom();
    if (this->mBackgroundVisible) {
        auto bk = Moho::CD3DBatchTexture::FromSolidColor(this->mBackgroundColor);
        batcher->SetTexture(bk);
        LODWORD(a3.mV0.z) = this->mAlphaMask;
        a4.mV0.z = a3.mV0.z;
        arg0.mV0.z = a3.mV0.z;
        result_4.z = a3.mV0.z;
        *(float *)chText = v57;
        a3.mV1.z = v57;
        a4.mV1.z = LazyValue;
        LODWORD(a3.mV0.x) = v67._Myres;
        a3.mV0.y = 0.0;
        a3.mV1.x = 0.0;
        a3.mV1.y = 1.0;
        LODWORD(a4.mV0.x) = v67._Myres;
        a4.mV0.y = 0.0;
        a4.mV1.x = 1.0;
        a4.mV1.y = 1.0;
        arg0.mV0.x = Advance;
        arg0.mV0.y = 0.0;
        arg0.mV1.x = 1.0;
        arg0.mV1.y = 0.0;
        result = LazyValue;
        result_4.x = Advance;
        result_4.y = 0.0;
        v62.info = 0;
        v62.count.pi_ = 0;
        batcher->DrawQuad(
            (const struct Moho::CD3DPrimBatcher::Vertex *)&result,
            (struct Moho::CD3DPrimBatcher::Vertex *)&a3.mV1.z,
            (const struct Moho::CD3DPrimBatcher::Vertex *)chText,
            (struct Moho::CD3DPrimBatcher::Vertex *)&a4.mV1.z
        );
    }
    mClipOffset = (Wm3::Vector3f *)this->mClipOffset;
    mSelectionStart = this->mSelectionStart;
    int v58 = this->mClipOffset + this->mClipLength;
    x = v58;
    if (mSelectionStart >= v58) {
        mSelectionStart = v58;
    }
    if (mSelectionStart > this->mClipOffset) {
        v8 = this->mText._Myres < 0x10;
        LODWORD(a9) = mSelectionStart - (_DWORD)mClipOffset;
        p_arg0 = mClipOffset;
        p_y = (Moho::CD3DPrimBatcher *)(mSelectionStart - (_DWORD)mClipOffset);
        v55 = COERCE_FLOAT(&p_y);
        if ( v8 )
        p_Bx = &this->mText._Bx;
        else
        p_Bx = (std::string::_Bxty *)this->mText._Bx._Ptr;
        gpg::STR_Utf8SubString((std::string *)&arg0.mV1.z, p_Bx->_Buf, (int)p_arg0, SLODWORD(a9));
        v72 = 1;
        Myfirstiter = v67._Myfirstiter;
        v55 = COERCE_FLOAT(&a9);
        if ( v67._Mysize < 0x10 )
        Myfirstiter = (struct _Iterator_base *)&v67;
        Advance = this->mFont->GetAdvance((const char *)Myfirstiter, SLODWORD(v51));
        v58 = func_GetLazyValue(&this->mTopLV);
        v58 = this->mFont->mFontMetrics.mAscent + v58;
        if (this->mDropShadow) {
            v12 = COERCE_FLOAT(this->MultiplyAlpha(this->mForegroundColor & 0xFF000000));
            Ptr = (std::string::_Bxty *)v67._Bx._Ptr;
            v51 = NaN_227;
            a9 = v12;
            p_arg0 = (Wm3::Vector3f *)&arg0;
            arg0.mV0.y = -1.0;
            p_y = a2;
            a3.mV0.x = v55 + 1.0;
            p_a4 = &a4;
            arg0.mV0.x = 0.0;
            arg0.mV0.z = 0.0;
            a4.mV0.x = 1.0;
            a4.mV0.y = 0.0;
            a4.mV0.z = 0.0;
            a3.mV0.y = v58 + 1.0;
            a3.mV0.z = 0.0;
            LODWORD(result_4.x) = &p_a4;
            if ( v67._Myres < 0x10 )
                Ptr = &v67._Bx;
            this->mFont->Render(&a3.mV0, &a4.mV0, &result_4, (char *)Ptr->_Buf, p_y, p_arg0, a9, v51);
        }
        a3.mV0.y = -1.0;
        a4.mV0.x = 1.0;
        v51 = NaN_227;
        arg0.mV0.x = v55;
        a3.mV0.x = 0.0;
        a3.mV0.z = 0.0;
        a4.mV0.y = 0.0;
        a4.mV0.z = 0.0;
        arg0.mV0.y = v58;
        arg0.mV0.z = 0.0;
        v15 = COERCE_FLOAT(this->MultiplyAlpha(this->mForegroundColor));
        v16 = (std::string::_Bxty *)v67._Bx._Ptr;
        a9 = v15;
        p_arg0 = (Wm3::Vector3f *)&a3;
        p_y = a2;
        p_a4 = a2;
        LODWORD(result_4.x) = &p_a4;
        if ( v67._Myres < 0x10 )
        v16 = &v67._Bx;
        Moho::CD3DFont::Render(&arg0.mV0, &a4.mV0, &result_4, this->mFont, (char *)v16->_Buf, p_y, p_arg0, a9, v51);
        vars0 = -1;
        v55 = Advance + v55;
        if ( v67._Myres >= 0x10 )
        operator delete((void *)v67._Bx._Ptr);
        x = result;
    }
    v17 = this->mSelectionStart;
    if ( v17 < this->mClipOffset )
        v17 = this->mClipOffset;
    mSelectionEnd = this->mSelectionEnd;
    if ( mSelectionEnd >= SLODWORD(x) )
        mSelectionEnd = LODWORD(x);
    if ( mSelectionEnd > v17) {
        v8 = this->mText._Myres < 0x10;
        LODWORD(v51) = mSelectionEnd - v17;
        LODWORD(a9) = v17;
        p_arg0 = (Wm3::Vector3f *)(mSelectionEnd - v17);
        LODWORD(result_4.x) = &p_arg0;
        if ( v8 )
        v19 = &this->mText._Bx;
        else
        v19 = (std::string::_Bxty *)this->mText._Bx._Ptr;
        gpg::STR_Utf8SubString(&v67, v19->_Buf, SLODWORD(a9), SLODWORD(v51));
        vars0 = 2;
        v20 = (std::string::_Bxty *)v67._Bx._Ptr;
        LODWORD(result_4.x) = &v51;
        if ( v67._Myres < 0x10 )
        v20 = &v67._Bx;
        v21 = this->mFont->GetAdvance(v20->_Buf, SLODWORD(v52));
        result = v21;
        if (this->mBackgroundVisible) {
            v22 = Moho::CD3DBatchTexture::FromSolidColor(this->mHighlightBackgroundColor);
            Moho::CD3DPrimBatcher::SetTexture(v22, a2);
            LOBYTE(retaddr) = 2;
            Moho::WeakPtr_CD3DBatchTexture::Release((boost::shared_ptr_CD3DBatchTexture *)&result_4.y);
            LODWORD(a3.mV1.y) = this->mAlphaMask;
            a4.mV1.y = a3.mV1.y;
            arg0.mV1.y = a3.mV1.y;
            v62.count.pi_ = (boost::detail::sp_counted_impl_p *)LODWORD(a3.mV1.y);
            a4.mV0.y = result + Advance;
            arg0.mV0.y = result + Advance;
            a3.mV0.y = Advance;
            a3.mV0.z = v69;
            a3.mV1.x = 0.0;
            a3.mV1.z = 0.0;
            arg0.mV0.x = 1.0;
            a4.mV0.z = v69;
            a4.mV1.x = 0.0;
            a4.mV1.z = 1.0;
            a3.mV0.x = 1.0;
            arg0.mV0.z = v58;
            arg0.mV1.x = 0.0;
            arg0.mV1.z = 1.0;
            v67._Myfirstiter = 0;
            result_4.y = Advance;
            result_4.z = v58;
            v62.info = 0;
            *(_DWORD *)chText = 0;
            a4.mV0.x = 0.0;
            Moho::CD3DPrimBatcher::DrawQuad(
                a2,
                (const struct Moho::CD3DPrimBatcher::Vertex *)&result_4.y,
                (struct Moho::CD3DPrimBatcher::Vertex *)&arg0.mV0.y,
                (struct Moho::CD3DPrimBatcher::Vertex *)&a4.mV0.y,
                (struct Moho::CD3DPrimBatcher::Vertex *)&a3.mV0.y);
        }
        v57 = func_GetLazyValue(&this->mTopLV);
        v11 = !this->mDropShadow;
        v23 = v57 + this->mFont->mFontMetrics.mAscent;
        v58 = v23;
        if ( !v11 )
        {
        v24 = COERCE_FLOAT(this->MultiplyAlpha(this->mHighlightForegroundColor & 0xFF000000));
        v25 = (char *)*((_DWORD *)&v67._Bx._Ptr + 1);
        v52 = NaN_227;
        v51 = v24;
        LODWORD(a9) = &a3.mV0.y;
        a3.mV0.z = -1.0;
        p_arg0 = (Wm3::Vector3f *)a2;
        arg0.mV0.y = Advance + 1.0;
        p_y = (Moho::CD3DPrimBatcher *)&a4.mV0.y;
        a3.mV0.y = 0.0;
        a3.mV1.x = 0.0;
        a4.mV0.y = 1.0;
        a4.mV0.z = 0.0;
        a4.mV1.x = 0.0;
        arg0.mV0.z = v58 + 1.0;
        arg0.mV1.x = 0.0;
        LODWORD(result_4.y) = &p_y;
        if ( v68 < 0x10 )
            v25 = (char *)&v67._Bx._Buf[4];
        Moho::CD3DFont::Render(
            (Wm3::Vector3f *)&arg0.mV0.y,
            (Wm3::Vector3f *)&a4.mV0.y,
            (Wm3::Vector3f *)&result_4.y,
            this->mFont,
            v25,
            (Moho::CD3DPrimBatcher *)p_arg0,
            (Wm3::Vector3f *)LODWORD(a9),
            v51,
            v52);
        v23 = v58;
        }
        mHighlightForegroundColor = this->mHighlightForegroundColor;
        a3.mV0.z = -1.0;
        v52 = NaN_227;
        a4.mV0.y = 1.0;
        a3.mV0.y = 0.0;
        a3.mV1.x = 0.0;
        a4.mV0.z = 0.0;
        a4.mV1.x = 0.0;
        arg0.mV0.y = Advance;
        arg0.mV0.z = v23;
        arg0.mV1.x = 0.0;
        v27 = COERCE_FLOAT(this->MultiplyAlpha(mHighlightForegroundColor));
        v28 = (char *)*((_DWORD *)&v67._Bx._Ptr + 1);
        v51 = v27;
        LODWORD(a9) = &a3.mV0.y;
        p_arg0 = (Wm3::Vector3f *)a2;
        p_y = a2;
        LODWORD(result_4.y) = &p_y;
        if ( v68 < 0x10 )
        v28 = (char *)&v67._Bx._Buf[4];
        Moho::CD3DFont::Render(
        (Wm3::Vector3f *)&arg0.mV0.y,
        (Wm3::Vector3f *)&a4.mV0.y,
        (Wm3::Vector3f *)&result_4.y,
        this->mFont,
        v28,
        (Moho::CD3DPrimBatcher *)p_arg0,
        (Wm3::Vector3f *)LODWORD(a9),
        v51,
        v52);
        retaddr = -1;
        Advance = result + Advance;
        if ( v68 >= 0x10 )
        operator delete(*((void **)&v67._Bx._Ptr + 1));
        x = result_4.x;
    }
    v29 = *(float *)&this->mSelectionEnd;
    if ( SLODWORD(v29) < this->mClipOffset )
        v29 = *(float *)&this->mClipOffset;
    if ( SLODWORD(x) > SLODWORD(v29) )
    {
        v8 = this->mText._Myres < 0x10;
        LODWORD(v52) = LODWORD(x) - LODWORD(v29);
        v51 = v29;
        a9 = v29;
        LODWORD(result_4.y) = &a9;
        if ( v8 )
        v30 = &this->mText._Bx;
        else
        v30 = (std::string::_Bxty *)this->mText._Bx._Ptr;
        gpg::STR_Utf8SubString((std::string *)&v67._Bx, v30->_Buf, SLODWORD(v51), SLODWORD(v52));
        retaddr = 4;
        v31 = (const char *)*((_DWORD *)&v67._Bx._Ptr + 1);
        LODWORD(result_4.y) = &v52;
        if ( v68 < 0x10 )
        v31 = &v67._Bx._Buf[4];
        Moho::CD3DFont::GetAdvance(this->mFont, v31, SLODWORD(v53[0]));
        v58 = func_GetLazyValue(&this->mTopLV);
        v11 = !this->mDropShadow;
        result = this->mFont->mFontMetrics.mAscent + v58;
        if ( !v11 )
        {
        v32 = COERCE_FLOAT(this->MultiplyAlpha(this->mForegroundColor & 0xFF000000));
        v33 = (char *)*((_DWORD *)&v67._Bx._Ptr + 2);
        v53[0] = NaN_227;
        v52 = v32;
        v51 = COERCE_FLOAT((struct Moho::CD3DPrimBatcher::Vertex *)&a3.mV0.z);
        a3.mV1.x = -1.0;
        LODWORD(a9) = a2;
        arg0.mV0.z = v57 + 1.0;
        p_arg0 = (Wm3::Vector3f *)&a4.mV0.z;
        a3.mV0.z = 0.0;
        a3.mV1.y = 0.0;
        a4.mV0.z = 1.0;
        a4.mV1.x = 0.0;
        a4.mV1.y = 0.0;
        arg0.mV1.x = result + 1.0;
        arg0.mV1.y = 0.0;
        LODWORD(result_4.z) = &p_arg0;
        if ( LODWORD(v69) < 0x10 )
            v33 = (char *)&v67._Bx._Buf[8];
        this->mFont->Render(
            (Wm3::Vector3f *)&arg0.mV0.z,
            (Wm3::Vector3f *)&a4.mV0.z,
            (Wm3::Vector3f *)&result_4.z,
            v33,
            (Moho::CD3DPrimBatcher *)LODWORD(a9),
            (Wm3::Vector3f *)LODWORD(v51),
            v52,
            v53[0]);
        }
        v34 = this->mForegroundColor;
        a3.mV1.x = -1.0;
        a4.mV0.z = 1.0;
        v53[0] = NaN_227;
        arg0.mV0.z = v57;
        a3.mV0.z = 0.0;
        a3.mV1.y = 0.0;
        a4.mV1.x = 0.0;
        a4.mV1.y = 0.0;
        arg0.mV1.x = result;
        arg0.mV1.y = 0.0;
        v35 = COERCE_FLOAT(Moho::CMauiEdit::MultiplyAlpha(this, v34));
        v36 = (char *)*((_DWORD *)&v67._Bx._Ptr + 2);
        v52 = v35;
        v51 = COERCE_FLOAT((struct Moho::CD3DPrimBatcher::Vertex *)&a3.mV0.z);
        LODWORD(a9) = a2;
        p_arg0 = (Wm3::Vector3f *)a2;
        LODWORD(result_4.z) = &p_arg0;
        if ( LODWORD(v69) < 0x10 )
        v36 = (char *)&v67._Bx._Buf[8];
        Moho::CD3DFont::Render(
        (Wm3::Vector3f *)&arg0.mV0.z,
        (Wm3::Vector3f *)&a4.mV0.z,
        (Wm3::Vector3f *)&result_4.z,
        this->mFont,
        v36,
        (Moho::CD3DPrimBatcher *)LODWORD(a9),
        (Wm3::Vector3f *)LODWORD(v51),
        v52,
        v53[0]);
        if ( LODWORD(v69) >= 0x10 )
        operator delete(*((void **)&v67._Bx._Ptr + 2));
    }
    if (this->mCaretVisible) {
        mCaretPosition = this->mCaretPosition;
        v38 = this->mClipOffset;
        if ( mCaretPosition >= v38 && mCaretPosition <= v38 + this->mClipLength) {
            v70 = this->mCaretColor | (this->mCaretCurColor << 24);
            v39 = *(float *)&this->mClipOffset;
            LODWORD(v53[0]) = mCaretPosition - LODWORD(v39);
            v52 = v39;
            v51 = v39;
            LODWORD(result_4.z) = &v51;
            func_cstrSet((const char **)&v51, &this->mText);
            gpg::STR_Utf8SubString(&v71, (const char *)LODWORD(v51), SLODWORD(v52), SLODWORD(v53[0]));
            v40 = (std::string::_Bxty *)v71._Bx._Ptr;
            LODWORD(result_4.z) = v53;
            if ( v71._Myres < 0x10 )
                v40 = &v71._Bx;
            result = Moho::CD3DFont::GetAdvance(this->mFont, v40->_Buf, SLODWORD(v53[1]));
            v41 = func_GetLazyValue(&this->mLeftLV);
            result_4.z = v41 + result;
            v42 = func_GetLazyValue(&this->mTopLV);
            mFont = this->mFont;
            result_4.x = v42;
            mAscent = mFont->mFontMetrics.mAscent;
            v45 = this->mForegroundColor;
            result_4.y = result_4.z + 1.0;
            v60 = (float)(mFont->mFontMetrics.mDescent + mAscent) + result_4.x;
            v46 = Moho::CD3DBatchTexture::FromSolidColor(&v62, v45);
            Moho::CD3DPrimBatcher::SetTexture(v46, (Moho::CD3DPrimBatcher *)5);
            Moho::WeakPtr_CD3DBatchTexture::Release(&v62);
            a3.mV1.x = result_4.z;
            LODWORD(arg0.mV0.x) = v71._Myfirstiter;
            LODWORD(a3.mV0.x) = v71._Myfirstiter;
            v67._Myfirstiter = v71._Myfirstiter;
            LODWORD(a4.mV0.x) = v71._Myfirstiter;
            arg0.mV0.z = 1.0;
            a4.mV1.x = result_4.y;
            a3.mV0.y = 1.0;
            a3.mV0.z = 1.0;
            arg0.mV1.x = result_4.y;
            *(_QWORD *)v67._Bx._Buf = 1065353216LL;
            a3.mV1.y = v60;
            a3.mV1.z = 0.0;
            arg0.mV0.y = 0.0;
            a4.mV1.y = v60;
            a4.mV1.z = 0.0;
            arg0.mV1.y = result_4.x;
            arg0.mV1.z = 0.0;
            v62.info = (Moho::CD3DBatchTexture *)LODWORD(result_4.z);
            v62.count.pi_ = (boost::detail::sp_counted_impl_p *)LODWORD(result_4.x);
            *(_DWORD *)chText = 0;
            a4.mV0.y = 0.0;
            a4.mV0.z = 0.0;
            Moho::CD3DPrimBatcher::DrawQuad(
                (Moho::CD3DPrimBatcher *)5,
                (const struct Moho::CD3DPrimBatcher::Vertex *)&v62,
                (const struct Moho::CD3DPrimBatcher::Vertex *)&arg0.mV1,
                (const struct Moho::CD3DPrimBatcher::Vertex *)&a4.mV1,
                (const struct Moho::CD3DPrimBatcher::Vertex *)&a3.mV1);
            std::string::~string(&v71._Bx);
        }
    }
}

// 0x00790470
bool Moho::CMauiEdit::HandleEvent(const Moho::SMauiEventData &ev) {
    Moho::EMauiEventType evType = ev.mEventType;
    if (evType == MET_Char) {
        this->HandleKeyEvent(ev);
    } else if (MET_ButtonPress <= evType && evType <= MET_ButtonDClick) {
        this->HandleClickEvent(ev);
    }
    return false;
}

// 0x0078F720
void Moho::CMauiEdit::OnFrame(float delta) {
    this->RunScript("OnFrame", &delta);
    this->mCaretTime += delta;
    if (this->mCaretTime > this->mCaretCycleSeconds) {
        this->mCaretTime = 0.0;
    }
    float halfCycle = this->mCaretCycleSeconds * 0.5;
    float interp;
    if (this->mCaretTime < halfCycle) {
        interp = this->mCaretTime / halfCycle;
    } else {
        interp = (this->mCaretCycleSeconds - this->mCaretTime) / halfCycle;
    }
    this->mCaretCurColor = (this->mCaretCycleOnColor - this->mCaretCycleOffColor) * interp + this->mCaretCycleOffColor;
}

// 0x0078F330
void Moho::CMauiEdit::LoseFocus() {
    this->mCaretVisible = false;
    Moho::CScriptObject *cur = Moho::Maui_CurrentFocusControl.ListGetPrev();
    if (cur == this) {
        func_AcquireFocus(nullptr, true);
    }
}

// 0x007915A0
void Moho::CMauiEdit::LoseKeyboardFocus() {
    this->LoseFocus();
    this->RunScript("OnLoseKeyboardFocus");
}

// 0x0078F280
void Moho::CMauiEdit::Dump() {
    this->Moho::CMauiControl::Dump();
    gpg::Logf("CMauiEdit");
    gpg::Logf(
        "FG Color = %#08X BG Color = %#08X HLFG Color = %#08X HLBG Color = %#08X Show BG = %s MaxChars = %d",
        this->mForegroundColor,
        this->mBackgroundColor,
        this->mHighlightForegroundColor,
        this->mHighlightBackgroundColor,
        this->mBackgroundVisible ? "true" : "false",
        this->mMaxChars
    );
    gpg::Logf("Current Text = %s", this->mText.c_str());
}

// 0x007913A0
void Moho::CMauiEdit::DragMove(Moho::SMauiEventData *ev) {
    float x = ev->mMouseX - this->GetLeft();
    std::string select = gpg::STR_Utf8SubString(this->GetText(), this->mClipOffset, this->mClipLength);
    int nearest = this->mFont->GetNearestCharacterIndex(select.c_str(), x) + this->mClipOffset;
    if (nearest == this->mDragStart) {
        this->mSelectionStart = 0;
        this->mSelectionEnd = 0;
    } else {
        if (nearest >= this->mDragStart) {
            this->mSelectionStart = this->mDragStart;
            this->mSelectionEnd = nearest;
        } else {
            this->mSelectionStart = nearest;
            this->mSelectionEnd = this->mDragStart;
        }
        this->SetCaretPosition(nearest);
    }
}

// 0x007914C0
void Moho::CMauiEdit::DragRelease(Moho::SMauiEventData *ev) {
    float x = ev->mMouseX - this->GetLeft();
    std::string select = gpg::STR_Utf8SubString(this->GetText(), this->mClipOffset, this->mClipLength);
    int endChar = this->mClipOffset + this->mFont->GetNearestCharacterIndex(this->GetText(), x);
    if (endChar == this->mDragStart) {
        this->mSelectionStart = 0;
        this->mSelectionEnd = 0;
    }
}


// 0x0078EC10
Moho::color_t Moho::CMauiEdit::MultiplyAlpha(Moho::color_t col) {
    return (col & 0xFFFFFF) - ((unsigned int) (__int64) (this->mAlpha * -255.0) << 24);
}

// 0x0078F380
void Moho::CMauiEdit::SetText(std::string *text) {
    std::string oldText{this->mText};
    std::string newText = gpg::STR_Utf8SubString(text->c_str(), 0, this->mMaxChars);
    this->mText = gpg::STR_Utf8Len(newText.c_str());
    this->mClipOffset = 0;
    this->mClipLength = gpg::STR_Utf8Len(this->mText.c_str());
    this->SetClipOffsetRight(0);
    if (! this->mDoingCallback) {
        this->mDoingCallback = true;
        this->TextChanged(&this->mText, &oldText);
        this->mDoingCallback = false;
    }
}

// 0x0078F4C0
void Moho::CMauiEdit::ClearText() {
    std::string oldText{this->mText};
    this->mText.clear();
    this->mCaretPosition = 0;
    this->mClipLength = 0;
    this->mClipOffset = 0;
    this->mSelectionStart = 0;
    this->mSelectionEnd = 0;
    if (! this->mDoingCallback) {
        this->mDoingCallback = true;
        this->TextChanged(&this->mText, &oldText);
        this->mDoingCallback = false;
    }
}

// 0x0078F570
void Moho::CMauiEdit::SetMaxChars(int max) {
    this->mMaxChars = max;
    if (this->mMaxChars < this->GetTextLen()) {
        this->mText.assign(gpg::STR_Utf8SubString(this->GetText(), 0, this->mMaxChars));
    }
}

// 0x007904D0
bool Moho::CMauiEdit::EnterPressed() {
    this->RunScript("OnEnterPressed", &this->mText);
}

// 0x007904F0
bool Moho::CMauiEdit::EscPressed() {
    this->RunScript("OnEscPressed", &this->mText);
}

// 0x00790510
void Moho::CMauiEdit::NonTextKeyPressed(int key, const Moho::SMauiEventData &ev) {
    LuaPlus::LuaObject obj = func_CreateLuaEvent(this->mLuaObj.GetActiveState(), ev);
    this->RunScript("OnNonTextKeyPressed", &key, &obj);
}

// 0x00790B40
void Moho::CMauiEdit::DeleteCharAtCaret(bool toRight) {
    std::string oldText{this->mText};
    if (this->mSelectionStart != this->mSelectionEnd) {
        this->DeleteSelection(false);
        this->SetClipOffsetLeft(this->mClipOffset);
    } else if (toRight) {
        if (this->mCaretPosition != this->GetTextLen()) {
            int pos = gpg::STR_Utf8ByteOffset(this->GetText(), this->mCaretPosition);
            int end = gpg::STR_Utf8ByteOffset(this->GetText(), this->mCaretPosition + 1);
            this->mText.erase(pos, end - pos);
        }
        this->SetClipOffsetLeft(this->mClipOffset);
    } else if (this->mCaretPosition) {
        int pos = gpg::STR_Utf8ByteOffset(this->GetText(), this->mCaretPosition - 1);
        int end = gpg::STR_Utf8ByteOffset(this->GetText(), this->mCaretPosition);
        this->mText.erase(pos, end - pos);
        this->MoveCaretLeft(1);
        if (this->mCaretPosition > 0) {
            if (this->mCaretPosition == this->mClipOffset) {
                this->SetClipOffsetLeft(this->mClipOffset - 1);
            }
        }
    }
    if (! this->mDoingCallback) {
        this->mDoingCallback = true;
        this->TextChanged(&this->mText, &oldText);
        this->mDoingCallback = false;
    }
}

// 0x007906F0
std::string Moho::CMauiEdit::GetSelection() {
    std::string select{};
    if (this->mSelectionStart != this->mSelectionEnd) {
        int start = this->mSelectionStart;
        int end = this->mSelectionEnd;
        select = gpg::STR_Utf8SubString(this->GetText(), start, end - start;
    }
    return select;
}

// 0x007907B0
void Moho::CMauiEdit::ClearSelection() {
    this->ReplaceSelection(gpg::STR_WideToUtf8(L""));
}

// 0x00790590
void Moho::CMauiEdit::DeleteSelection(bool supress) {
    if (this->mSelectionStart != this->mSelectionEnd) {
        std::string oldText{this->mText};
        int len = gpg::STR_Utf8Len(this->mText.c_str());
        if (len > this->mSelectionEnd) {
            len = this->mSelectionEnd;
        }
        this->mSelectionEnd = len;
        this->mCaretPosition = this->mSelectionStart;
        int startPos = gpg::STR_Utf8ByteOffset(this->mText.c_str(), this->mSelectionStart);
        int endPos = gpg::STR_Utf8ByteOffset(this->mText.c_str(), this->mSelectionEnd);
        this->mText.erase(startPos, endPos - startPos);
        if (this->mClipOffset > this->mCaretPosition) {
            this->mClipOffset = this->mCaretPosition;
        }
        this->mSelectionStart = 0;
        this->mSelectionEnd = 0;
        if (! supress && ! this->mDoingCallback) {
            this->mDoingCallback = true;
            this->TextChanged(&this->mText, &oldText);
            this->mDoingCallback = false;
        }
    }
}

// 0x00790830
void Moho::CMauiEdit::ReplaceSelection(const std::string &str) {
    std::string replacement{};
    if (this->GetTextLen() + gpg::STR_Utf8Len(str.c_str()) <= this->mMaxChars) {
        replacement = str;
    } else {
        if (this->GetTextLen() >= this->mMaxChars) {
            return;
        }

        replacement = gpg::STR_Utf8SubString(str.c_str(), 0, this->mMaxChars - this->GetTextLen());
    }
    this->DeleteSelection(false);
    std::string oldText{this->mText};
    if (! replacement.empty()) {
        if (this->mCaretPosition == this->GetTextLen()) {
            this->mText.append(replacement);
            this->mCaretPosition += gpg::STR_Utf8Len(replacement.c_str());
            this->mClipLength += gpg::STR_Utf8Len(replacement.c_str());
            this->SetClipOffsetRight(0);
        } else {
            int pos = gpg::STR_Utf8ByteOffset(this->GetText(), (int)this->mCaretPosition);
            this->mText.insert(pos, replacement);
            this->mCaretPosition += gpg::STR_Utf8Len(replacement.c_str());
            if (this->mCaretPosition >= this->mClipOffset + this->mClipLength) {
                this->SetClipOffsetRight(this->GetTextLen() - this->mCaretPosition);
            } else {
                this->SetClipOffsetLeft(this->mClipOffset);
            }
        }
    }
    if (! this->mDoingCallback) {
        this->mDoingCallback = true;
        this->TextChanged(&this->mText, &oldText);
        this->mDoingCallback = false;
    }
}

// 0x0x00790D20
void Moho::CMauiEdit::SetClipOffsetRight(int pos) {
    if (this->mFont == nullptr) {
        return;
    }

    if (this->mFont->GetAdvance(this->GetText(), pos) <= this->GetWidth()) {
        this->mClipLength = this->GetTextLen();
    } else {
        if (pos > this->GetTextLen()) {
            pos = this->GetTextLen();
        }
        int charsAfter = this->GetTextLen() - pos;
        int clipLen = this->GetClipThreshold(pos);
        this->mClipLength = clipLen;
        this->mClipOffset = charsAfter - clipLen;
    }
}


// 0x0x00790D20
void Moho::CMauiEdit::SetClipOffsetLeft(int pos) {
    this->mClipOffset = pos;
    if (this->mFont == nullptr) {
        return;
    }

    if (this->mFont->GetAdvance(this->GetText(), pos) <= this->GetWidth()) {
        this->mClipLength = this->GetTextLen();
    } else {
        if (pos > this->GetTextLen()) {
            pos = this->GetTextLen();
        }
        this->mClipLength = this->GetClipThreshold(pos);
    }
}

// 0x007911B0
void Moho::CMauiEdit::SetCaretPosition(int pos) {
    if (pos < this->mCaretPosition) {
        this->mCaretPosition = pos;
        if (pos < this->mClipOffset) {
            this->SetClipOffsetLeft(pos);
        }
    } else if (pos > this->mCaretPosition) {
        if (pos >= this->GetTextLen()) {
            pos = this->GetTextLen();
        }
        this->mCaretPosition = pos;
        if (pos >= this->mClipOffset + this->mClipLength) {
            this->SetClipOffsetLeft(this->GetTextLen() - pos);
        }
    }
}

// 0x00791250
void Moho::CMauiEdit::MoveCaretLeft(int amt) {
    if (this->mCaretPosition != 0) {
        if (amt >= this->mCaretPosition) {
            amt = this->mCaretPosition;
        }
        this->SetCaretPosition(this->mCaretPosition - amt);
    }
}

// 0x00791270
void Moho::CMauiEdit::MoveCaretRight(int amt) {
    this->SetCaretPosition(this->mCaretPosition + amt);
}

// 0x00791290
void Moho::CMauiEdit::MoveSelectionLeft(int amt) {
    if (this->mSelectionStart == this->mSelectionEnd) {
        this->mSelectionEnd = this->mCaretPosition;
        this->mSelectionStart = this->mCaretPosition;
    }
    int oldPos = this->mCaretPosition;
    this->MoveCaretLeft(amt);
    if (this->mCaretPosition != oldPos) {
        if (this->mSelectionStart == this->mCaretPosition) {
            this->mSelectionStart = 0;
            this->mSelectionEnd = 0;
        } else if (this->mSelectionStart >= this->mCaretPosition) {
            this->mSelectionStart = this->mCaretPosition;
        } else {
            this->mSelectionEnd = this->mCaretPosition;
        }
    }
}

// 0x00791310
void Moho::CMauiEdit::MoveSelectionRight(int amt) {
    if (this->mSelectionStart == this->mSelectionEnd) {
        this->mSelectionEnd = this->mCaretPosition;
        this->mSelectionStart = this->mCaretPosition;
    }
    int oldPos = this->mCaretPosition;
    this->MoveCaretRight(amt);
    if (this->mCaretPosition != oldPos) {
        if (this->mSelectionStart == this->mCaretPosition) {
            this->mSelectionStart = 0;
            this->mSelectionEnd = 0;
        } else if (this->mSelectionEnd > this->mCaretPosition) {
            this->mSelectionStart = this->mCaretPosition;
        } else {
            this->mSelectionEnd = this->mCaretPosition;
        }
    }
}

// 0x007915C0
void Moho::CMauiEdit::HandleClickEvent(const Moho::SMauiEventData &ev) {
    if ((ev.mModifiers & MEM_Left) != 0) {
        if (this->mIsEnabled) {
            this->mCaretVisible = true;
            func_AcquireFocus(this, true);
        }
        int x = ev.mMouseX - this->GetLeft();
        std::string text = gpg::STR_Utf8SubString(this->GetText(), this->mClipOffset, this->mClipLength);
        int nearest = this->mFont->GetNearestCharacterIndex(this->GetText(), x);
        if (ev.mEventType == MET_ButtonPress) {
            func_PostDragger(this->mRootFrame, this, ev);
            int dragStart = this->mClipOffset + nearest;
            this->mDragStart = dragStart;
            this->SetCaretPosition(dragStart);
        } else {
            int wordStart = gpg::STR_GetWordStartIndex(text, nearest);
            int word = text.find_first_of(gpg::STR_GetWhitespaceCharacters(), wordStart);
            if (word == std::string::npos) {
                word = gpg::STR_Utf8Len(text.c_str());
            }
            int caretPos = this->mClipOffset + word;
            this->mSelectionStart = this->mClipOffset + wordStart;
            this->mSelectionEnd = caretPos;
            this->SetCaretPosition(caretPos);
        }
    }
}

// 0x00791780
void Moho::CMauiEdit::HandleKeyEvent(const Moho::SMauiEventData &ev) {
    switch (ev.mKeyCode) {
        case MKEY_HOME: {
            if ((ev.mModifiers & MEM_Shift) != 0) {
                if (this->mCaretPosition != 0) {
                    this->MoveSelectionLeft(mCaretPosition);
                }
            } else {
                this->mCaretPosition = 0;
                this->mSelectionStart = 0;
                this->mSelectionEnd = 0;
                this->SetClipOffsetLeft(0);
            }
            break;
        }
        case MKEY_LEFT: {
            if ((ev.mModifiers & MEM_Shift) != 0 && (ev.mModifiers & MEM_Ctrl) != 0) {
                int pos = this->mCaretPosition - gpg::STR_GetWordStartIndex(this->mText, this->mCaretPosition);
                if (pos != 0) {
                    this->MoveSelectionLeft(pos);
                }
            } else if ((ev.mModifiers & MEM_Shift) != 0) {
                this->MoveSelectionLeft(1);
            } else {
                if ((ev.mModifiers & MEM_Ctrl) != 0) {
                    int pos = gpg::STR_GetWordStartIndex(this->mText, this->mCaretPosition);
                    this->SetCaretPosition(pos);
                } else {
                    this->MoveCaretLeft(1);
                }
                this->mSelectionStart = 0;
                this->mSelectionEnd = 0;
            }
            break;
        }
        case MKEY_RIGHT: {
            if ((ev.mModifiers & MEM_Shift) != 0 && (ev.mModifiers & MEM_Ctrl) != 0) {
                int pos = gpg::STR_GetNextWordStartIndex(this->mText, this->mCaretPosition) - this->mCaretPosition;
                if (pos != 0) {
                    this->MoveSelectionRight(pos);
                }
            }
            if ((ev.mModifiers & MEM_Shift) != 0) {
                this->MoveSelectionRight(1);
            } else {
                if ((ev.mModifiers & MEM_Ctrl) != 0) {
                    int pos = gpg::STR_GetNextWordStartIndex(this->mText, this->mCaretPosition);
                    this->SetCaretPosition(pos);
                } else {
                    this->MoveCaretRight(1);
                }
                this->mSelectionStart = 0;
                this->mSelectionEnd = 0;
            }
            break;
        }
        case MKEY_INSERT: {
            if ((ev.mModifiers & MEM_Shift) != 0) {
                this->ReplaceSelection(Moho::WIN_GetClipboardText());
            } else if ((ev.mModifiers & MEM_Ctrl) != 0) {
                this->CopyToClipboard();
            }
            break;
        }
        case MKEY_END: {
            if ((ev.mModifiers & MEM_Shift) != 0) {
                int pos = this->GetTextLen() - this->mCaretPosition;
                if (pos != 0) {
                    this->MoveSelectionRight(pos);
                }
            } else {
                this->mCaretPosition = this->GetTextLen();
                this->mSelectionStart = 0;
                this->mSelectionEnd = 0;
                this->SetClipOffsetRight(0);
            }
            break;
        }
        case MKEY_BACK: {
            if ((ev.mModifiers & MEM_Alt) != 0) {
                break;
            }
            if ((ev.mModifiers & MEM_Ctrl) == 0) {
                this->DeleteCharAtCaret(false);
                this->DeleteSelection(true);
                break;
            }
            int pos = this->mCaretPosition - gpg::STR_GetWordStartIndex(this->mText, this->mCaretPosition);
            if (pos != 0) {
                this->MoveSelectionLeft(pos);
            }
            this->DeleteSelection(true);
            break;
        }
        case MKEY_RETURN: {
            if (! this->EnterPressed()) {
                this->ClearText();
            }
            break;
        }
        case MKEY_ESCAPE: {
            if (! this->EscPressed()) {
                if (! this->mText.empty()) {
                    this->ClearText();
                } else {
                    this->LoseFocus();
                }
            }
            break;
        }
        case MKEY_DELETE: {
            if ((ev.mModifiers & MEM_Shift) != 0) {
                this->CopyToClipboard();
            } else if ((ev.mModifiers & MEM_Ctrl) != 0) {
                int pos = gpg::STR_GetNextWordStartIndex(this->mText, this->mCaretPosition) - this->mCaretPosition;
                if (pos != 0) {
                    this->MoveSelectionRight(pos);
                }
            } else {
                this->DeleteCharAtCaret(true);
            }
            this->DeleteSelection(true);
            break;
        }
        default: {
            if ((ev.mModifiers & MEM_Ctrl) != 0) {
                if (ev.mKeyCode == 3) {
                    this->CopyToClipboard();
                } else if (ev.mKeyCode == 22) {
                    this->ReplaceSelection(Moho::WIN_GetClipboardText());
                } else if (ev.mKeyCode == 24) {
                    this->CopyToClipboard();
                    this->DeleteSelection(true);
                }
            } else if (ev.mKeyCode <= MKEY_START) {
                int code = ev.mKeyCode;
                if (! this->CharPressed(&code)) {
                    this->ClearSelection();
                }
            } else {
                bool success = false;
                int code = wxCharCodeWXToMSW(ev.mKeyCode, &success);
                this->NonTextKeyPressed(code, ev);
            }
            break;
        }
    }
}

// 0x00794F20
void Moho::CMauiEdit::TextChanged(std::string *newText, std::string *oldText) {
    this->RunScript("OnTextChanged", newText, oldText);
}

// 0x007953D0
bool Moho::CMauiEdit::CharPressed(int *keycode) {
    this->RunScript("OnCharPressed", keycode);
}
