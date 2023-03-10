#ifndef CFD_FLUX_CALCULATORS_HPP
#define CFD_FLUX_CALCULATORS_HPP

#include <Eigen/Core>

#include "cfd/functions.hpp"
#include "cfd/problem_parameters.hpp"

namespace cfd {

/**
 * @brief Riemann-solver-based flux calculator
 *
 * @tparam SpacialReconstructor
 * @tparam RiemannSolver
 */
template <typename SpacialReconstructor, typename RiemannSolver>
class RiemannFluxCalculator {
 public:
  RiemannFluxCalculator(const ProblemParameters& params)
      : reconstructor_{params}, solver_{params} {}

  /**
   * @brief Compute flux
   *
   * @param U Conservation variables
   * @return Flux
   */
  template <typename Derived>
  Eigen::MatrixXd compute(const Eigen::MatrixBase<Derived>& U) const noexcept {
    using Eigen::MatrixXd;
    const MatrixXd Ul = reconstructor_.calc_left(U);
    const MatrixXd Ur = reconstructor_.calc_right(U);
    return solver_.calc_flux(Ul, Ur);
  }

 private:
  SpacialReconstructor reconstructor_;
  RiemannSolver solver_;
};

class LaxWendroffFluxCalculator {
 public:
  LaxWendroffFluxCalculator(const ProblemParameters& params)
      : dx_{params.dx},
        gamma_{params.specific_heat_ratio},
        n_boundary_cells_{params.n_bounary_cells},
        n_domain_cells_{params.n_domain_cells} {}

  /**
   * @brief Compute flux
   *
   * @param U Conservation variables
   * @param dt Time step length
   * @return Flux
   */
  template <typename Derived>
  Eigen::MatrixXd compute(const Eigen::MatrixBase<Derived>& U,
                          double dt) const noexcept {
    using Eigen::ArrayXd, Eigen::MatrixXd, Eigen::seqN, Eigen::Map;

    Map<const ArrayXd> rho(&U(0, 0), U.rows());
    Map<const ArrayXd> rhou(&U(0, 1), U.rows());
    Map<const ArrayXd> rhoE(&U(0, 2), U.rows());

    const auto i = n_boundary_cells_;
    const auto n = n_domain_cells_ + 1;
    const auto rng1 = seqN(i, n);
    const auto rng2 = seqN(i - 1, n);
    const auto alpha = (0.5 * dt / dx_);

    const ArrayXd rho_m =
        0.5 * (rho(rng1) + rho(rng2)) - alpha * (rhou(rng1) - rhou(rng2));
    const ArrayXd u = rhou / rho;
    const ArrayXd p = calc_pressure(rho, u, rhoE, gamma_);
    const ArrayXd f = rhou * u + p;
    const ArrayXd rhou_m =
        0.5 * (rhou(rng1) + rhou(rng2)) - alpha * (f(rng1) - f(rng2));
    const ArrayXd g = u * (rhoE + p);
    const ArrayXd rhoE_m =
        0.5 * (rhoE(rng1) + rhoE(rng2)) - alpha * (g(rng1) - g(rng2));
    const ArrayXd u_m = rhou_m / rho_m;
    const ArrayXd p_m = calc_pressure(rho_m, u_m, rhoE_m, gamma_);

    MatrixXd F(n, 3);
    F.col(0) = rhou_m.matrix();
    F.col(1) = (rhou_m * u_m + p_m).matrix();
    F.col(2) = (u_m * (rhoE_m + p_m)).matrix();
    return F;
  }

 private:
  double dx_;             ///> Grid length
  double gamma_;          ///> Specific heat ratio
  int n_boundary_cells_;  ///> Number of boundary cells
  int n_domain_cells_;    ///> Number of domain cells
};

}  // namespace cfd

#endif  // CFD_FLUX_CALCULATORS_HPP