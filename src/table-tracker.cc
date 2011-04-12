// Copyright 2011, Thomas Moulard, Olivier Stasse, JRL, CNRS/AIST
//
// This file is part of evart-to-corba.
// evart-to-corba is free software: you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation, either version 3 of
// the License, or (at your option) any later version.
//
// evart-to-corba is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Lesser Public License for more details.  You should have
// received a copy of the GNU Lesser General Public License along with
// evart-to-corba. If not, see <http://www.gnu.org/licenses/>.

#include <cmath>
#include <iostream>
#include <stdexcept>
#include <utility>

#include <boost/foreach.hpp>
#include <boost/format.hpp>

#include <boost/numeric/ublas/io.hpp>

// uBlas extension
#include "storage_adaptors.hpp"

#include <evart-client.h>

#include "corba-signal.hh"

#define ENABLE_DEBUG
#include "debug.hh"

#include "table-tracker.hh"

namespace ublas = boost::numeric::ublas;

typedef ublas::vector<double> vector_t;
typedef ublas::matrix<double> matrix_t;

static const double TABLE_WIDTH = 0.3;
static const double TABLE_LENGTH = 0.3;
static const double TABLE_HEIGHT = 0.25;

TableTracker::TableTracker (Application& app)
  : TrackedBody (app, "tablePosition", 0, 4),
    front_ (0),
    leftUp_ (1),
    rightUp_ (2)
{}

TableTracker::~TableTracker ()
{}

void
TableTracker::computeSignal (const evas_msg_t* msg)
{
/**
       ----o  BR      x <---------------o
      |    |                            |
      |    |                           \|/
      |    |                            y
   FL o----o  BL
**/

  vector_t frontL = ublas::make_vector_from_pointer    (3, msg->body_markers.markers[0]);
  vector_t backR  = ublas::make_vector_from_pointer    (3, msg->body_markers.markers[1]);
  vector_t backL  = ublas::make_vector_from_pointer    (3, msg->body_markers.markers[2]);



  double originX = 0.001*(frontL[0] + backR[0])/2.0;
  double originY = 0.001*(frontL[1] + backR[1])/2.0;
  double theta;
  if( fabs(frontL[0] - backL[0]) != 0.0 ) theta = atan( (frontL[1] - backL[1]) / (frontL[0] - backL[0]));
  else theta = M_PI / 2.0;

  signalOutput_->length (3);
  signalOutput_[0] = originX;
  signalOutput_[1] = originY;
  signalOutput_[2] = theta;

  LOG ()
    << "-> "
    << originX << " | "
    << originY << " | "
    << theta * 180. / M_PI << " deg" << std::endl;
}
