#ifndef CFD_SPACIAL_RECONSTRUCTION_SCHEMES_HPP
#define CFD_SPACIAL_RECONSTRUCTION_SCHEMES_HPP

#include "cfd/problem_parameters.hpp"
#include "cfd/variables.hpp"

namespace cfd {

class FirstOrderSpacialReconstructor {
 public:
  FirstOrderSpacialReconstructor(int n_boundary_cells, int n_domain_cells)
      : n_boundary_cells_{n_boundary_cells},
        n_domain_cells_{n_boundary_cells} {}

  FirstOrderSpacialReconstructor(const ProblemParameters& params)
      : n_boundary_cells_{params.n_bounary_cells},
        n_domain_cells_{params.n_domain_cells} {}

  ConservativeVariables calc_left(
      const ConservativeVariables& vars) const noexcept;

  ConservativeVariables calc_right(
      const ConservativeVariables& vars) const noexcept;

 private:
  int n_boundary_cells_;
  int n_domain_cells_;
};

class LaxWendroffSpacialReconstructor {
 public:
  LaxWendroffSpacialReconstructor(double dx, double gamma, int n_boundary_cells,
                                  int n_domain_cells)
      : dx_{dx},
        gamma_{gamma},
        n_boundary_cells_{n_boundary_cells},
        n_domain_cells_{n_boundary_cells} {}

  LaxWendroffSpacialReconstructor(const ProblemParameters& params)
      : dx_{params.dx},
        gamma_{params.specific_heat_ratio},
        n_boundary_cells_{params.n_bounary_cells},
        n_domain_cells_{params.n_domain_cells} {}

  FluxVectors calc_flux(const ConservativeVariables& vars,
                        double dt) const noexcept;

 private:
  double dx_;
  double gamma_;
  int n_boundary_cells_;
  int n_domain_cells_;
};

}  // namespace cfd

#endif  // CFD_SPACIAL_RECONSTRUCTION_SCHEMES_HPP