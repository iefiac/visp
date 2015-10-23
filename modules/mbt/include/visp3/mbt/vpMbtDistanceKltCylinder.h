/****************************************************************************
 *
 * $Id: vpMbtDistanceKltCylinder.h 4661 2014-02-10 19:34:58Z fspindle $
 *
 * This file is part of the ViSP software.
 * Copyright (C) 2005 - 2014 by INRIA. All rights reserved.
 *
 * This software is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * ("GPL") version 2 as published by the Free Software Foundation.
 * See the file LICENSE.txt at the root directory of this source
 * distribution for additional information about the GNU GPL.
 *
 * For using ViSP with software that can not be combined with the GNU
 * GPL, please contact INRIA about acquiring a ViSP Professional
 * Edition License.
 *
 * See http://www.irisa.fr/lagadic/visp/visp.html for more information.
 *
 * This software was developed at:
 * INRIA Rennes - Bretagne Atlantique
 * Campus Universitaire de Beaulieu
 * 35042 Rennes Cedex
 * France
 * http://www.irisa.fr/lagadic
 *
 * If you have questions regarding the use of this file, please contact
 * INRIA at visp@inria.fr
 *
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 * Description:
 * Klt cylinder, containing points of interest.
 *
 * Authors:
 * Aurelien Yol
 *
 *****************************************************************************/

#ifndef vpMbtDistanceKltCylinder_h
#define vpMbtDistanceKltCylinder_h

#include <visp3/core/vpConfig.h>

#if (defined(VISP_HAVE_OPENCV) && (VISP_HAVE_OPENCV_VERSION >= 0x020100))

#include <map>

#include <visp3/core/vpPolygon3D.h>
#include <visp3/vision/vpKltOpencv.h>
#include <visp3/core/vpPlane.h>
#include <visp3/core/vpDisplay.h>
#include <visp3/core/vpGEMM.h>
#include <visp3/vision/vpHomography.h>
#include <visp3/mbt/vpMbHiddenFaces.h>
#include <visp3/core/vpCylinder.h>
#include <visp3/core/vpCircle.h>

/*!
  \class vpMbtDistanceKltCylinder

  \brief Implementation of a polygon of the model containing points of interest. It is used by the model-based tracker KLT, and hybrid.

  \warning This class is only available if OpenCV is installed, and used.

  \ingroup group_mbt_features
*/
class VISP_EXPORT vpMbtDistanceKltCylinder
{
private:
  //! Pose at initialisation
  vpHomogeneousMatrix c0Mo;
  //! First extremity of the cylinder (used for display)
  vpPoint p1Ext;
  //! Second extremity of the cylinder (used for display)
  vpPoint p2Ext;
  //! Cylinder
  vpCylinder cylinder;
  //! The upper circle limiting the cylinder (used for display)
  vpCircle circle1;
  //! The lower circle limiting the cylinder (used for display)
  vpCircle circle2;
  //! Initial points and their ID
  std::map<int, vpImagePoint> initPoints;
  //! Initial points and their ID
  std::map<int, vpPoint> initPoints3D;
  //! Current points and their ID
  std::map<int, vpImagePoint> curPoints;
  //! Current points ID and their indexes
  std::map<int, int> curPointsInd;
  //! number of points detected
  unsigned int nbPointsCur;
  //! initial number of points
  unsigned int nbPointsInit;
  //! Minimal number of points to be tracked
  unsigned int minNbPoint;
  //! Boolean to know if there is enough point to be tracked
  bool enoughPoints;
  //! Camera parameters
  vpCameraParameters cam;
  //! Boolean to specify if the klt points have to be tracked or not
  bool isTrackedKltCylinder;

public:
  //! Pointer to the polygon that define a face
  std::vector<int> listIndicesCylinderBBox;
  //! Pointer to the list of faces
  vpMbHiddenFaces<vpMbtPolygon> *hiddenface;
  //! Use scanline rendering
  bool useScanLine;

private:
  double              computeZ(const double &x, const double &y);
  bool                isTrackedFeature(const int id);

public:
                      vpMbtDistanceKltCylinder();
  virtual             ~vpMbtDistanceKltCylinder();

  void                buildFrom(const vpPoint &p1, const vpPoint &p2, const double &r);

  unsigned int        computeNbDetectedCurrent(const vpKltOpencv& _tracker);
  void                computeInteractionMatrixAndResidu(const vpHomogeneousMatrix &cMc0, vpColVector& _R, vpMatrix& _J);

  void                display(const vpImage<unsigned char> &I, const vpHomogeneousMatrix &cMo, const vpCameraParameters &cam, const vpColor col, const unsigned int thickness = 1, const bool displayFullModel = false);
  void                display(const vpImage<vpRGBa> &I, const vpHomogeneousMatrix &cMo, const vpCameraParameters &cam, const vpColor col, const unsigned int thickness = 1, const bool displayFullModel = false);

  void                displayPrimitive(const vpImage<unsigned char>& _I);
  void                displayPrimitive(const vpImage<vpRGBa>& _I);

  /*!
    Get the camera parameters of the face.

    \return cam : the camera parameters of the face.
  */
  inline vpCameraParameters& getCameraParameters(){ return cam; }

  inline std::map<int, vpImagePoint>& getCurrentPoints() {return curPoints; }

  inline std::map<int, int>& getCurrentPointsInd() {return curPointsInd; }

  /*!
    Get the number of point that was belonging to the face at the initialisation

    \return the number of initial point.

    \sa getCurrentNumberPoints()
  */
  inline unsigned int getInitialNumberPoint() const { return nbPointsInit;}
  /*!
    Get the number of points detected in the last image.

    \warning To have the real number of points, the function computeNbDetectedCurrent()
    must be called first.

    \return the number of points detected in the current image.

    \sa getInitialNumberPoint()
  */
  inline unsigned int getCurrentNumberPoints() const {return nbPointsCur;}

  inline  bool        hasEnoughPoints() const {return enoughPoints;}

  /*!
   Return if the klt cylinder is used for tracking.

   \return True if it is used, False otherwise.
  */
  inline  bool        isTracked() const {return isTrackedKltCylinder;}

  void                init(const vpKltOpencv& _tracker, const vpHomogeneousMatrix &cMo);

  void                removeOutliers(const vpColVector& weight, const double &threshold_outlier);

  /*!
    Set the camera parameters

    \param _cam : the new camera parameters
  */
  virtual inline void setCameraParameters(const vpCameraParameters& _cam){ cam = _cam; }

  /*!
    Set if the klt cylinder has to be considered during tracking phase.

    \param track : True if is has to be tracked, False otherwise.
  */
  inline void         setTracked(const bool& track) {this->isTrackedKltCylinder = track;}

#if (VISP_HAVE_OPENCV_VERSION >= 0x020408)
  void updateMask(cv::Mat &mask, unsigned char _nb = 255, unsigned int _shiftBorder = 0);
#else
  void updateMask(IplImage* mask, unsigned char _nb = 255, unsigned int _shiftBorder = 0);
#endif
};

#endif

#endif // VISP_HAVE_OPENCV