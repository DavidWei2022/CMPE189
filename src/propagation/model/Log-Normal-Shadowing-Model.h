#ifndef LOG_NORMAL_SHADOWING_MODEL_H
#define LOG_NORMAL_SHADOWING_MODEL_H

#include "ns3/object.h"
#include "ns3/random-variable-stream.h"
#include "propagation-loss-model.h"

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

// class MobilityModel;

// class PropagationLossModel : public Object
// {
//   public:
//     /**
//      * Get the type ID.
//      * \brief Get the type ID.
//      * \return the object TypeId
//      */
//     static TypeId GetTypeId();

//     PropagationLossModel();
//     ~PropagationLossModel() override;

//     // Delete copy constructor and assignment operator to avoid misuse
//     PropagationLossModel(const PropagationLossModel&) = delete;
//     PropagationLossModel& operator=(const PropagationLossModel&) = delete;

//     /**
//      * \brief Enables a chain of loss models to act on the signal
//      * \param next The next PropagationLossModel to add to the chain
//      *
//      * This method of chaining propagation loss models only works commutatively
//      * if the propagation loss of all models in the chain are independent
//      * of transmit power.
//      */
//     void SetNext(Ptr<PropagationLossModel> next);

//     /**
//      * \brief Gets the next PropagationLossModel in the chain of loss models
//      * that act on the signal.
//      * \returns The next PropagationLossModel in the chain
//      *
//      * This method of chaining propagation loss models only works commutatively
//      * if the propagation loss of all models in the chain are independent
//      * of transmit power.
//      */
//     Ptr<PropagationLossModel> GetNext();

//     /**
//      * Returns the Rx Power taking into account all the PropagationLossModel(s)
//      * chained to the current one.
//      *
//      * \param txPowerDbm current transmission power (in dBm)
//      * \param a the mobility model of the source
//      * \param b the mobility model of the destination
//      * \returns the reception power after adding/multiplying propagation loss (in dBm)
//      */
//     double CalcRxPower(double txPowerDbm, Ptr<MobilityModel> a, Ptr<MobilityModel> b) const;

//     /**
//      * If this loss model uses objects of type RandomVariableStream,
//      * set the stream numbers to the integers starting with the offset
//      * 'stream'. Return the number of streams (possibly zero) that
//      * have been assigned.  If there are PropagationLossModels chained
//      * together, this method will also assign streams to the
//      * downstream models.
//      *
//      * \param stream the stream index offset start
//      * \return the number of stream indices assigned by this model
//      */
//     int64_t AssignStreams(int64_t stream);

//   protected:
//     /**
//      * Assign a fixed random variable stream number to the random variables used by this model.
//      *
//      * Subclasses must implement this; those not using random variables
//      * can return zero.
//      *
//      * \param stream first stream index to use
//      * \return the number of stream indices assigned by this model
//      */
//     virtual int64_t DoAssignStreams(int64_t stream) = 0;

//   private:
//     /**
//      * PropagationLossModel.
//      *
//      * \param txPowerDbm current transmission power (in dBm)
//      * \param a the mobility model of the source
//      * \param b the mobility model of the destination
//      * \returns the reception power after adding/multiplying propagation loss (in dBm)
//      */
//     virtual double DoCalcRxPower(double txPowerDbm,
//                                  Ptr<MobilityModel> a,
//                                  Ptr<MobilityModel> b) const = 0;

//     Ptr<PropagationLossModel> m_next; //!< Next propagation loss model in the list
// };


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