#############################################################################
#
# $Id$
#
# Copyright (C) 1998-2010 Inria. All rights reserved.
#
# This software was developed at:
# IRISA/INRIA Rennes
# Projet Lagadic
# Campus Universitaire de Beaulieu
# 35042 Rennes Cedex
# http://www.irisa.fr/lagadic
#
# This file is part of the ViSP toolkit.
#
# This file may be distributed under the terms of the Q Public License
# as defined by Trolltech AS of Norway and appearing in the file
# LICENSE included in the packaging of this file.
#
# Licensees holding valid ViSP Professional Edition licenses may
# use this file in accordance with the ViSP Commercial License
# Agreement provided with the Software.
#
# This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
# WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
#
# Contact visp@irisa.fr if any conditions of this licensing are
# not clear to you.
#
# Description:
# Try to find linux/parport.h.h for parallel port usage.
# Once run this will define: 
#
# PARPORT_FOUND
#
# Authors:
# Fabien Spindler
#
#############################################################################

FIND_PATH(PARPORT_INCLUDE_DIR linux/parport.h
  /usr/include 
  /usr/src/linux/include)
  #MESSAGE("DBG PARPORT_INCLUDE_DIR=${PARPORT_INCLUDE_DIR}")  

MARK_AS_ADVANCED(
  PARPORT_INCLUDE_DIR
)
      
IF(PARPORT_INCLUDE_DIR)
  SET(PARPORT_FOUND TRUE)
ELSE(PARPORT_INCLUDE_DIR)
  SET(PARPORT_FOUND FALSE)
ENDIF(PARPORT_INCLUDE_DIR)
  