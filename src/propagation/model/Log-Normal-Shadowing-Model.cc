#include "ns3/Log-Normal-Shadowing-Model.h"
#include "ns3/propagation-loss-model.h"
#include "ns3/boolean.h"
#include "ns3/double.h"
#include "ns3/log.h"
#include "ns3/mobility-model.h"
#include "ns3/pointer.h"
#include "ns3/string.h"

#include <cmath>
#include <string>
namespace ns3
{
NS_LOG_COMPONENT_DEFINE("LogNormalShadowingModel");

//------------------------------------------------------------------------------------------------------------------------------------//
NS_OBJECT_ENSURE_REGISTERED(LogNormalShadowingModel);

TypeId
LogNormalShadowingModel::GetTypeId()
{
    static TypeId tid =
        TypeId("ns3::LogNormalShadowingModel")
            .SetParent<PropagationLossModel>()
            .SetGroupName("Propagation")
            .AddConstructor<LogNormalShadowingModel>()
            .AddAttribute("Exponent",
                          "The exponent of the Path Loss propagation model",
                          DoubleValue(3.0),
                          MakeDoubleAccessor(&LogNormalShadowingModel::m_exponent),
                          MakeDoubleChecker<double>())
            .AddAttribute("ReferenceDistance",
                          "The distance at which the reference loss is calculated (m)",
                          DoubleValue(1.0),
                          MakeDoubleAccessor(&LogNormalShadowingModel::m_referenceDistance),
                          MakeDoubleChecker<double>())
            .AddAttribute("ReferenceLoss",
                          "The reference loss at reference distance (dB). (Default is Friis at 1m "
                          "with 5.15 GHz)",
                          DoubleValue(46.6777),
                          MakeDoubleAccessor(&LogNormalShadowingModel::m_referenceLoss),
                          MakeDoubleChecker<double>())
            .AddAttribute(
                            "Variable",
                            "The random variable used to pick a loss every time CalcRxPower is invoked.",
                            StringValue("ns3::NormalRandomVariable[Mean=0|Variance=2]"),
                            MakePointerAccessor(&LogNormalShadowingModel::m_variable),
                            MakePointerChecker<RandomVariableStream>());
    return tid;
}


LogNormalShadowingModel::LogNormalShadowingModel()
{
}

void
LogNormalShadowingModel::SetPathLossExponent(double n)
{
    m_exponent = n;
}
// double
// LogNormalShadowingModel::GetRandom()const{
//     return m_variable;
// }
void
LogNormalShadowingModel::SetReference(double referenceDistance, double referenceLoss)
{
    m_referenceDistance = referenceDistance;
    m_referenceLoss = referenceLoss;
}

double
LogNormalShadowingModel::GetPathLossExponent() const
{
    return m_exponent;
}

double
LogNormalShadowingModel::DoCalcRxPower(double txPowerDbm,
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
     * rx = 10 * log (Pr0(tx)) - n * 10 * log (d/d0) + x
     *
     * Pr0: rx power at reference distance d0 (W)
     * d0: reference distance: 1.0 (m)
     * d: distance (m)
     * tx: tx power (dB)
     * rx: dB
     * x:GaussianDistributedRandomVariable
     * Which, in our case is:
     *
     * rx = rx0(tx) - 10 * n * log (d/d0) + x
     */
    double pathLossDb = 10 * m_exponent * std::log10(distance / m_referenceDistance);
    double rxc = -m_referenceLoss - pathLossDb;
    double GaussianDistributedRandomVariable= m_variable->GetValue();
    NS_LOG_DEBUG("distance=" << distance << "m, reference-attenuation=" << -m_referenceLoss
                             << "dB, "
                             << "attenuation coefficient=" << rxc << "db");
    return txPowerDbm + rxc+GaussianDistributedRandomVariable;
}

int64_t
LogNormalShadowingModel::DoAssignStreams(int64_t stream)
{
    return 0;
}
//------------------------------------------------------------------------------------------------------------------------------------//
}