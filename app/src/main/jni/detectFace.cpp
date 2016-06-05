//
// Created by mayur on 18/05/16.
//

#include <jni.h>
#include <stdio.h>
#include "opencv2/core/core.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/features2d/features2d.hpp"

using namespace std;
using namespace cv;

int toGray(Mat img, Mat& gray);
Ptr<FastFeatureDetector> detector;
CascadeClassifier face_cascade;

extern "C" {

JNIEXPORT jint JNICALL Java_mayur_resistor_resistorfinder_MainActivity_convertNativeGray(JNIEnv*, jobject, jlong addrRgba, jlong addrGray);
JNIEXPORT jint JNICALL Java_mayur_resistor_resistorfinder_MainActivity_setupDetectFace(JNIEnv* jenv, jobject, jstring jFileName);
JNIEXPORT jint JNICALL Java_mayur_resistor_resistorfinder_MainActivity_detectFace(JNIEnv* jenv, jobject, jlong addrRgba, jlong addrRetVal);
JNIEXPORT jint JNICALL Java_mayur_resistor_resistorfinder_MainActivity_featureDetect(JNIEnv* jenv, jobject, jlong addrRgba, jlong addrRetVal);
JNIEXPORT jint JNICALL Java_mayur_resistor_resistorfinder_MainActivity_setupFeatureDetect(JNIEnv* jenv, jobject);

JNIEXPORT jint JNICALL Java_mayur_resistor_resistorfinder_MainActivity_setupDetectFace(JNIEnv* jenv, jobject, jstring jFileName){
    const char* jnamestr = jenv->GetStringUTFChars(jFileName, NULL);
    string stdFileName(jnamestr);

    face_cascade.load(stdFileName);
}

JNIEXPORT jint JNICALL Java_mayur_resistor_resistorfinder_MainActivity_setupFeatureDetect(JNIEnv* jenv, jobject){

 detector = FastFeatureDetector::create();

 detector->setThreshold(50);

 return 1;

}

JNIEXPORT jint JNICALL Java_mayur_resistor_resistorfinder_MainActivity_featureDetect(JNIEnv* jenv, jobject, jlong addrRgba, jlong addrRetVal){

    Mat& mRgb = *(Mat*)addrRgba;
    Mat& mRetVal = *(Mat*)addrRetVal;

    std::vector<KeyPoint> keypoints_1;

    cvtColor(mRgb, mRgb, CV_RGB2GRAY);

    detector->detect( mRgb, keypoints_1 );

    drawKeypoints( mRgb, keypoints_1, mRetVal, Scalar::all(-1), DrawMatchesFlags::DEFAULT );

    return 1;
}

JNIEXPORT jint JNICALL Java_mayur_resistor_resistorfinder_MainActivity_convertNativeGray(JNIEnv*, jobject, jlong addrRgba, jlong addrGray) {

    Mat& mRgb = *(Mat*)addrRgba;
    Mat& mGray = *(Mat*)addrGray;

    int conv;
    jint retVal;

    conv = toGray(mRgb, mGray);
    retVal = (jint)conv;

    return retVal;

}

JNIEXPORT jint JNICALL Java_mayur_resistor_resistorfinder_MainActivity_detectFace(JNIEnv* jenv, jobject, jlong addrRgba, jlong addrRetVal)
{
    Mat& mRgba = *(Mat*)addrRgba;
    Mat& retValMat = *(Mat*)addrRetVal;
    Mat gray;
    vector<Rect> faces;

    jint retVal;
    int faceFound=0;

    mRgba.copyTo(retValMat);

    cvtColor(mRgba, gray, CV_RGBA2GRAY);

    face_cascade.detectMultiScale(gray, faces, 2, 1,
                CV_HAAR_FIND_BIGGEST_OBJECT |  CV_HAAR_SCALE_IMAGE,
                Size(30, 30), Size(900, 900));

    if (faces.size() > 0)
    {
        int index;
        Rect face;
        for(index=0; index<faces.size(); index++){
            face = faces[index];
            rectangle(retValMat, face, Scalar(255, 0, 0), 3);
        }
        faceFound = 1;
    }
    else
    {
        faceFound = 0;
    }

    retVal = (jint)faceFound;

    return retVal;
}
}

int toGray(Mat img, Mat& gray)
{
    cvtColor(img, gray, CV_RGBA2GRAY); // Assuming RGBA input

    if (gray.rows == img.rows && gray.cols == img.cols)
    {
        return (1);
    }
    return(0);
}
