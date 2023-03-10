#ifndef CFD_FUNCTIONS_HPP
#define CFD_FUNCTIONS_HPP

#include <Eigen/Core>

namespace cfd {

/**
 * @brief Compute veolocity
 *
 * @param[in] rho Density
 * @param[in] rhou Momentum density
 * @return Velocity
 */
template <typename Derived1, typename Derived2>
Eigen::ArrayXd calc_velocity(const Eigen::ArrayBase<Derived1>& rho,
                             const Eigen::ArrayBase<Derived2>& rhou) noexcept {
  return rhou / rho;
}

/**
 * @brief Compute Pressure
 *
 * @param[in] rho Density
 * @param[in] u Velocity
 * @param[in] rhoE Total energy density
 * @param[in] gamma Specific heat ratio
 * @return Pressure
 */
template <typename Derived1, typename Derived2, typename Derived3>
Eigen::ArrayXd calc_pressure(const Eigen::ArrayBase<Derived1>& rho,
                             const Eigen::ArrayBase<Derived2>& u,
                             const Eigen::ArrayBase<Derived3>& rhoE,
                             double gamma) noexcept {
  return (gamma - 1.0) * (rhoE - 0.5 * rho * u.square());
}

/**
 * @brief Compute sonic veolocity
 *
 * @param[in] rho Density
 * @param[in] p Pressure
 * @return Sonic velocity
 */
template <typename Derived1, typename Derived2>
Eigen::ArrayXd calc_sonic_velocity(const Eigen::ArrayBase<Derived1>& rho,
                                   const Eigen::ArrayBase<Derived2>& p,
                                   double gamma) noexcept {
  return (gamma * p / rho).sqrt();
}

/**
 * @brief Compute enthalpy
 *
 * @param[in] rho Density
 * @param[in] rhoE Total energy density
 * @param[in] p Pressure
 * @return Enthalpy
 */
template <typename Derived1, typename Derived2, typename Derived3>
Eigen::ArrayXd calc_enthalpy(const Eigen::ArrayBase<Derived1>& rho,
                             const Eigen::ArrayBase<Derived2>& rhoE,
                             const Eigen::ArrayBase<Derived3>& p) noexcept {
  return (rhoE + p) / rho;
}

}  // namespace cfd

#endif  // CFD_FUNCTIONS_HPP