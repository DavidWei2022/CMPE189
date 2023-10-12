/*
 * Copyright (c) 2005,2006,2007 INRIA
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 * Contributions: Timo Bingmann <timo.bingmann@student.kit.edu>
 * Contributions: Tom Hewer <tomhewer@mac.com> for Two Ray Ground Model
 *                Pavel Boyko <boyko@iitp.ru> for matrix
 */

#include "propagation-loss-model.h"

#include "ns3/boolean.h"
#include "ns3/double.h"
#include "ns3/log.h"
#include "ns3/mobility-model.h"
#include "ns3/pointer.h"
#include "ns3/string.h"

#include <cmath>

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("PropagationLossModel");

// ------------------------------------------------------------------------- //

NS_OBJECT_ENSURE_REGISTERED(PropagationLossModel);

NS_OBJECT_ENSURE_REGISTERED(LogDistancePropagationLossModel);

TypeId
LogDistancePropagationLossModel::GetTypeId()
{
    static TypeId tid =
        TypeId("ns3::LogDistancePropagationLossModel")
            .SetParent<PropagationLossModel>()
            .SetGroupName("Propagation")
            .AddConstructor<LogDistancePropagationLossModel>()
            .AddAttribute("Exponent",
                          "The exponent of the Path Loss propagation model",
                          DoubleValue(3.0),
                          MakeDoubleAccessor(&LogDistancePropagationLossModel::m_exponent),
                          MakeDoubleChecker<double>())
            .AddAttribute("ReferenceDistance",
                          "The distance at which the reference loss is calculated (m)",
                          DoubleValue(1.0),
                          MakeDoubleAccessor(&LogDistancePropagationLossModel::m_referenceDistance),
                          MakeDoubleChecker<double>())
            .AddAttribute("ReferenceLoss",
                          "The reference loss at reference distance (dB). (Default is Friis at 1m "
                          "with 5.15 GHz)",
                          DoubleValue(46.6777),
                          MakeDoubleAccessor(&LogDistancePropagationLossModel::m_referenceLoss),
                          MakeDoubleChecker<double>());
    return tid;
}

LogDistancePropagationLossModel::LogDistancePropagationLossModel()
{
}

void
LogDistancePropagationLossModel::SetPathLossExponent(double n)
{
    m_exponent = n;
}

void
LogDistancePropagationLossModel::SetReference(double referenceDistance, double referenceLoss)
{
    m_referenceDistance = referenceDistance;
    m_referenceLoss = referenceLoss;
}

double
LogDistancePropagationLossModel::GetPathLossExponent() const
{
    return m_exponent;
}

double
LogDistancePropagationLossModel::DoCalcRxPower(double txPowerDbm,
                                               Ptr<MobilityModel> a,
                                               Ptr<MobilityModel> b) const
{
    double distance = a->GetDistanceFrom(b);
    if (distance <= m_referenceDistance)
    {
        return txPowerDbm - m_referenceLoss;
    }
    /**
     * The formula is:
     * rx = 10 * log (Pr0(tx)) - n * 10 * log (d/d0)
     *
     * Pr0: rx power at reference distance d0 (W)
     * d0: reference distance: 1.0 (m)
     * d: distance (m)
     * tx: tx power (dB)
     * rx: dB
     *
     * Which, in our case is:
     *
     * rx = rx0(tx) - 10 * n * log (d/d0)
     */
    double pathLossDb = 10 * m_exponent * std::log10(distance / m_referenceDistance);
    double rxc = -m_referenceLoss - pathLossDb;
    NS_LOG_DEBUG("distance=" << distance << "m, reference-attenuation=" << -m_referenceLoss
                             << "dB, "
                             << "attenuation coefficient=" << rxc << "db");
    return txPowerDbm + rxc;
}

int64_t
LogDistancePropagationLossModel::DoAssignStreams(int64_t stream)
{
    return 0;
}

// ------------------------------------------------------------------------- //

} // namespace ns3
