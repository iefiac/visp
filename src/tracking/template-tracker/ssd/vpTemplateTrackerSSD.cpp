/****************************************************************************
 *
 * $Id$
 *
 * This file is part of the ViSP software.
 * Copyright (C) 2005 - 2013 by INRIA. All rights reserved.
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
 * Template tracker.
 *
 * Authors:
 * Amaury Dame
 * Aurelien Yol
 * Fabien Spindler
 *
 *****************************************************************************/

#include <visp/vpTemplateTrackerSSD.h>

vpTemplateTrackerSSD::vpTemplateTrackerSSD(vpTemplateTrackerWarp *warp):vpTemplateTracker(warp)
{
  dW.resize(2,nbParam);
  G.resize(nbParam);
  H.resize(nbParam,nbParam);
  HLM.resize(nbParam,nbParam);

  temp.resize(nbParam);

  X1.resize(2);
  X2.resize(2);
  DI.resize(2);

  tG = NULL;
}

double vpTemplateTrackerSSD::getCost(const vpImage<unsigned char> &I,vpColVector &tp)
{
  double erreur=0;
  double IW,Tij;
  int i,j;
  double i2,j2;
  int Nbpoint=0;

  Warp->computeCoeff(tp);
  for(unsigned int point=0;point<templateSize;point++)
  {
    i=ptTemplate[point].y;
    j=ptTemplate[point].x;
    X1[0]=j;X1[1]=i;
    Warp->computeDenom(X1,tp);
    Warp->warpX(X1,X2,tp);

    j2=X2[0];i2=X2[1];
    if((i2>=0)&&(j2>=0)&&(i2<I.getHeight()-1)&&(j2<I.getWidth()-1))
    {
      Tij=ptTemplate[point].val;
      if(!blur)
        IW=I.getValue(i2,j2);
      else
        IW=BI.getValue(i2,j2);
      //IW=getSubPixBspline4(I,i2,j2);
      erreur+=((double)Tij-IW)*((double)Tij-IW);
      Nbpoint++;
    }
  }
  ratioPixelIn=(double)Nbpoint/(double)templateSize;

  if(Nbpoint==0)return 10e10;
  return erreur/Nbpoint;
}


double vpTemplateTrackerSSD::getSSD(vpImage<unsigned char> &I,vpColVector &tp)
{
  double erreur=0;
  double IW,Tij;
  int i,j;
  double i2,j2;
  int Nbpoint=0;

  if(pyrInitialised)
  {
    templateSize=templateSizePyr[0];
    ptTemplate=ptTemplatePyr[0];
  }

  Warp->computeCoeff(tp);
  for(unsigned int point=0;point<templateSize;point++)
  {
    i=ptTemplate[point].y;
    j=ptTemplate[point].x;
    X1[0]=j;X1[1]=i;
    Warp->computeDenom(X1,tp);
    Warp->warpX(X1,X2,tp);

    j2=X2[0];i2=X2[1];
    if((j2<I.getWidth()-1)&&(i2<I.getHeight()-1)&&(i2>0)&&(j2>0))
    {
      Tij=ptTemplate[point].val;
      IW=I.getValue(i2,j2);
      //IW=getSubPixBspline4(I,i2,j2);
      erreur+=((double)Tij-IW)*((double)Tij-IW);
      Nbpoint++;
    }
  }
  if(Nbpoint==0)return 10e10;
  return erreur/Nbpoint;
}