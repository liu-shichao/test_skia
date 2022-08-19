#include <iostream>
#include "SkBitmapProcShader.h"

#include "SkCanvas.h"
#include "SkBitmap.h"
#include "SkTypeface.h"

///////////////////////////////////////////////////////////////////////////////

#include "SkImageEncoder.h"
#include "tools/ToolUtils.h"
// #include "SkImageDecoder.h"
// #include "SkImageEncoder.h"
// #include "SkRect.h"


#include "include/core/SkCanvas.h"
#include "include/core/SkStream.h"
#include "modules/svg/include/SkSVGDOM.h"
#include "modules/svg/include/SkSVGNode.h"
#include "src/core/SkOSFile.h"
#include "src/utils/SkOSPath.h"
#include "src/xml/SkDOM.h"




#define BG_COLOR    0xFFDDDDDD


void circle(SkCanvas* canvas, int width, bool aa) {
    SkPaint paint;

    paint.setAntiAlias(aa);
    if (width < 0) {
        paint.setStyle(SkPaint::kFill_Style);
    } else {
        paint.setStyle(SkPaint::kStroke_Style);
        paint.setStrokeWidth(SkIntToScalar(width));
    }
    canvas->drawCircle(100, 100, 90.0f, paint);
    // if ((false)) { // avoid bit rot, suppress warning
    //     test_circlebounds(canvas);
    // }
}

void drawSvg(SkCanvas* canvas, SkString fPath, int width, int height) {
        SkFILEStream svgStream(fPath.c_str());
    if (!svgStream.isValid()) {
        SkDebugf("file not found: \"%s\"\n", fPath.c_str());
        return;
    }

    auto fDom = SkSVGDOM::MakeFromStream(svgStream);
    if (fDom) {
        fDom->setContainerSize(SkSize::Make(width, height));
    }

    if (fDom) {
        fDom->render(canvas);
    }
}

// void onSizeChange() override {
//     if (fDom) {
//         fDom->setContainerSize(SkSize::Make(this->width(), this->height()));
//     }

//     this->INHERITED::onSizeChange();
// }

// SkString name() override { return fLabel; }


int main()
{

    std::cout << "hello." << std::endl;
    SkBitmap bm;
    bm.allocN32Pixels(300, 645);
    SkCanvas canvas(bm);
    // SkScalar s = SkIntToScalar(1024) / 1024;
    // canvas.scale(s, s);
    canvas.save();
    canvas.drawColor(BG_COLOR);
    circle(&canvas, 4, true);
    drawSvg(&canvas, SkString("../123.svg"), 300, 200);
    canvas.restore();

    
    SkString str;
    str.printf("./slide_%zu.png", 1);
    ToolUtils::EncodeImageToFile(str.c_str(), bm, SkEncodedImageFormat::kPNG, 100);
    
    std::cout << str.c_str() << std::endl;
    // setBGColor(BG_COLOR);




    // const int w = 1080;
    // const int h = 1920;
    // /*准备目标图片和源图片*/
    // SkBitmap dst;
    // dst.allocPixels(SkImageInfo::Make(w, h, kN32_SkColorType, kPremul_SkAlphaType));
    // SkCanvas c(dst);

    // SkBitmap src;
    // SkImageDecoder::DecodeFile("test.jpg", &src);

    // /*各种绘制图片方法使用示例*/
    // {
    //     c.drawBitmap(src, 0, 0, NULL);
    // }

    // {
    //     c.drawSprite(src, 400, 400, NULL);
    // }
    // {
    //     SkRect dstR;
    //     r.set(29, 29, 100, 100);
    //     SkRect srcR;
    //     r.set(0,0,40,50);
    //     c.drawBitmapRectToRect(src, &srcR, dstR, NULL);
    // }
    // {
    //     SkMatrix m;
    //     m.setScale(1.4,4.3);
    //     c.drawBitmapMatrix(src, m, NULL);
    // }
    // {
    //     SkRect dstRect;
    //     dstRect.set(100,100,480,920);
    //     SkPaint paint;
    //     SkMatrix m;
    //     m.setScale(3.2, 4.1);
    //     SkShader* shader = CreateBitmapShader(src, SkShader::kRepeat_TileMode, SkShader::kRepeat_TileMode, m, NULL);
    //     paint.setShader(shader);
    //     SkSafeUnref(shader);
    //     c.drawRect(dstRect, paint);
    // }

    // /*输出图片*/
    // SkImageEncoder::EncodeFile("output.jpg", dst, SkImageEncoder::kJPEG_Type, 100);
    return 1;
}