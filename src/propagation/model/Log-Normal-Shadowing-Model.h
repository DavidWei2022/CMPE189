#ifndef LOG_NORMAL_SHADOWING_MODEL_H
#define LOG_NORMAL_SHADOWING_MODEL_H

#include "ns3/object.h"
#include "ns3/random-variable-stream.h"
#include "ns3/propagation-loss-model.h"

#include <map>

namespace ns3
{

/**
 * \defgroup propagation Propagation Models
 */

/**
 * \ingroup propagation
 * \ingroup tests
 * \defgroup propagation-tests Propagation module tests
 */

class LogNormalShadowingModel : public PropagationLossModel
{
  public:
    /**
     * \brief Get the type ID.
     * \return the object TypeId
     */
    static TypeId GetTypeId();
    LogNormalShadowingModel();

    // Delete copy constructor and assignment operator to avoid misuse
    LogNormalShadowingModel(const LogNormalShadowingModel&) = delete;
    LogNormalShadowingModel& operator=(const LogNormalShadowingModel&) = delete;

    /**
     * \param n the path loss exponent.
     * Set the path loss exponent.
     */
    void SetPathLossExponent(double n);
    /**
     * \returns the current path loss exponent.
     */
    double GetPathLossExponent() const;
    // double GetRandom() const;
    /**
     * Set the reference path loss at a given distance
     * \param referenceDistance reference distance
     * \param referenceLoss reference path loss
     */
    void SetReference(double referenceDistance, double referenceLoss);

  private:
    double DoCalcRxPower(double txPowerDbm,
                         Ptr<MobilityModel> a,
                         Ptr<MobilityModel> b) const override;

    int64_t DoAssignStreams(int64_t stream) override;

    /**
     *  Creates a default reference loss model
     * \return a default reference loss model
     */
    static Ptr<PropagationLossModel> CreateDefaultReference();

    double m_exponent;          //!< model exponent
    double m_referenceDistance; //!< reference distance
    double m_referenceLoss;     //!< reference loss
    Ptr<RandomVariableStream> m_variable; //!<refernce random
};

}

#endif