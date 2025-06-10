#ifndef MEMDC_H
#define MEMDC_H

class COffScreenDC : public CDC {
private:
    CBitmap m_bitmap;       // Offscreen bitmap
    CBitmap* m_oldBitmap;   // Original bitmap
    CDC* m_pDC;             // Original DC
    CRect m_rect;
    BOOL m_bMemDC;

public:
    COffScreenDC(CDC* pDC) : CDC(), m_oldBitmap(nullptr), m_pDC(pDC), m_bMemDC(FALSE) {
        ASSERT(pDC != nullptr);

        if (pDC->IsPrinting()) {
            m_bMemDC = FALSE;
            m_pDC = pDC;
        }
        else {
            m_bMemDC = TRUE;
            pDC->GetClipBox(&m_rect); // Get drawing area
            CreateCompatibleDC(pDC);
            m_bitmap.CreateCompatibleBitmap(pDC, m_rect.Width(), m_rect.Height());
            m_oldBitmap = SelectObject(&m_bitmap);
            SetWindowOrg(m_rect.TopLeft());
        }
    }

    ~COffScreenDC() {
        if (m_bMemDC) {
            // Copy from memory DC to screen
            m_pDC->BitBlt(m_rect.left, m_rect.top,
                m_rect.Width(), m_rect.Height(),
                this, m_rect.left, m_rect.top, SRCCOPY);
            SelectObject(m_oldBitmap);
        }
        else {
            // Non-memory DCs: do nothing
            m_oldBitmap = nullptr;
        }
    }

    // Allow transparent access to original DC
    operator CDC* () { return this; }
};

#endif
