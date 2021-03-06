#ifndef __Image_Process_H
#define __Image_Process_H

#include <vector>

#include <cvd/image.h>
#include <cvd/byte.h>
#include <cvd/utility.h>
#include <TooN/TooN.h>
#include <TooN/se2.h>
#include <TooN/se3.h>

#include "KeyFrame.h"
#include "ATANCamera.h"

using namespace CVD;
using namespace TooN;

class ImageProcess
{
public:
    bool mbMadeJacs;
    ImageProcess() {}
    CVD::Image<CVD::byte> GetImageROI(BasicImage<byte> &im, ImageRef irPos, CVD::ImageRef irSize);
    static double ShiTomasiScoreAtPoint(CVD::BasicImage<CVD::byte> &image, int nHalfBoxSize, CVD::ImageRef irCenter);
    inline int SSDAtPoint(CVD::BasicImage<byte> &im, const CVD::ImageRef &ir, CVD::Image<byte> &imTemplate, int nMaxSSD); // Score function
    double ZMSSD(CVD::Image<float> im1, CVD::Image<float> im2);
    void MakeJacs(CVD::Image<float> imTemplate, CVD::Image<Vector<2> > &imImageJacs);
};


class MiniPatch : public ImageProcess
{
public:
    MiniPatch():mirPatchSize(CVD::ImageRef(9,9)){}
    void SampleFromImage(CVD::ImageRef irPos, CVD::BasicImage<CVD::byte> &im);  // Copy pixels out of source image
    bool FindPatch(CVD::ImageRef &irPos,           // Find patch in a new image
                   CVD::BasicImage<CVD::byte> &im,
                   int nRange,
                   std::vector<CVD::ImageRef> &vCorners,
                   int nMaxSSD = 100000,//be important for tracking
                   std::vector<int> *pvRowLUT = NULL);

private:
    CVD::ImageRef mirPatchSize;
    CVD::Image<CVD::byte> mimOrigPatch;  // Original pixels
};


class SmallBlurryImage : public ImageProcess
{
public:

    CVD::Image<float> mimTemplate;
    CVD::Image<Vector<2> > mimImageJacs;

    SmallBlurryImage();
    SmallBlurryImage(KeyFrame &kf, double dBlur = 2.5);
    void MakeFromKF(KeyFrame &kf, double dBlur = 2.5);
    std::pair<SE2<>,double> IteratePosRelToTarget(SmallBlurryImage &other, int nIterations = 10);
    static SE3<> SE3fromSE2(SE2<> se2, ATANCamera camera);

protected:
    CVD::Image<CVD::byte> mimSmall;
    static CVD::ImageRef mirSize;
};

#endif
