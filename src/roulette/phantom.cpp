#include "roulette/phantom.h"
#include "roulette/math.h"
#include "roulette/photon.h"
#include "roulette/compound_table.h"

#include <random>
#include <numeric>
#include <fstream>

namespace roulette {
  Phantom::Phantom() :
    m_voxel_grid(),
    m_densities(std::make_shared<MatrixThreeTensor>())
  {
  }

  Phantom::Phantom(const rapidjson::Value& data) :
    m_voxel_grid(std::make_shared<const VoxelGrid>(data["voxel_grid"])),
    m_densities(std::make_shared<MatrixThreeTensor>(m_voxel_grid->nx(), m_voxel_grid->ny(), m_voxel_grid->nz(), data["density"].GetDouble())),
    m_delta_x((m_voxel_grid->vn()(0) - m_voxel_grid->v0()(0)) / this->nx()),
    m_delta_y((m_voxel_grid->vn()(1) - m_voxel_grid->v0()(1)) / this->ny()),
    m_delta_z((m_voxel_grid->vn()(2) - m_voxel_grid->v0()(2)) / this->nz())
  {
  }

  Phantom::Phantom(std::string filename) :
    m_densities(std::make_shared<MatrixThreeTensor>())
  {
    std::ifstream data_file;
    data_file.open(filename, std::ios::in | std::ios::binary);
    this->read(data_file);
    data_file.close();
  }

  Phantom::Phantom(std::shared_ptr<const VoxelGrid> voxel_grid, std::shared_ptr<MatrixThreeTensor> densities) : m_voxel_grid(voxel_grid),
    m_densities(densities),
    m_delta_x((m_voxel_grid->vn()(0) - m_voxel_grid->v0()(0)) / this->nx()),
    m_delta_y((m_voxel_grid->vn()(1) - m_voxel_grid->v0()(1)) / this->ny()),
    m_delta_z((m_voxel_grid->vn()(2) - m_voxel_grid->v0()(2)) / this->nz())
  {
  }

  Phantom::Phantom(const Phantom& original_phantom, std::tuple<int,int,int> voxelation_scale) {
    int sx = std::get<0>(voxelation_scale);
    int sy = std::get<1>(voxelation_scale);
    int sz = std::get<2>(voxelation_scale);

    assert(sx > 0 && sy > 0 && sz > 0);

    int nx = (original_phantom.nx() + sx - 1) / sx;
    int ny = (original_phantom.ny() + sy - 1) / sy;
    int nz = (original_phantom.nz() + sz - 1) / sz;

    m_delta_x = original_phantom.delta_x() * sx;
    m_delta_y = original_phantom.delta_y() * sy;
    m_delta_z = original_phantom.delta_z() * sz;

    ThreeVector vn(
      original_phantom.voxel_grid()->v0()(0) + nx*m_delta_x,
      original_phantom.voxel_grid()->v0()(1) + ny*m_delta_y,
      original_phantom.voxel_grid()->v0()(2) + nz*m_delta_z
    );

    m_voxel_grid = std::make_shared<VoxelGrid>(original_phantom.voxel_grid()->v0(), vn, nx, ny, nz);
    m_densities = std::make_shared<MatrixThreeTensor>(nx, ny, nz);

    m_compounds = std::vector<std::shared_ptr<const Compound>>();
    m_compounds.reserve(nx*ny*nz);

    std::random_device rd;
    std::mt19937 gen{rd()};

    // Fill in densities and compounds
    int x, y, z, xx, yy, zz, x2, y2, z2;
    for (z = 0; z < nz; ++z) {
      for (y = 0; y < ny; ++y) {
        for (x = 0; x < nx; ++x) {
          std::vector<double> densities;
          std::vector<std::shared_ptr<const Compound>> compounds;

          for (xx = 0; xx < sx; ++xx) {
            x2 = sx*x + xx;
            if (x2 >= original_phantom.nx()) {
              densities.push_back(0);
              continue;
            }

            for (yy = 0; yy < sy; ++yy) {
              y2 = sy*y + yy;
              if (y2 >= original_phantom.ny()) {
                densities.push_back(0);
                continue;
              }

              for (zz = 0; zz < sz; ++zz) {
                z2 = sz*z + zz;
                if (z2 >= original_phantom.nz()) {
                  densities.push_back(0);
                  continue;
                }

                densities.push_back((*original_phantom.densities())(x2, y2, z2));
                compounds.push_back(original_phantom.compound_ptr(x2, y2, z2));
              }
            }
          }

          (*m_densities)(x, y, z) = std::accumulate(densities.begin(), densities.end(), 0.0) / densities.size();
          // Select random compound within the existing compounds
          std::uniform_int_distribution<int> dist(0, compounds.size()-1);
          m_compounds.push_back(compounds[dist(gen)]);
        }
      }
    }
  }

  void Phantom::set_compound_map(const DensityCompoundMap& map) {
    if (!m_compounds.empty()) throw std::runtime_error("Cannot set compound map on phantom that has already had it set");

    m_compounds.reserve(this->nx() * this->ny() + this->nz());

    for (int z = 0; z < this->nz(); ++z) {
      for (int y = 0; y < this->ny(); ++y) {
        for (int x = 0; x < this->nx(); ++x) {
          m_compounds.push_back(map.compound_for_density((*m_densities)(x, y, z)));
        }
      }
    }
  }

  int Phantom::nx() const { return (*m_densities).nx(); }
  int Phantom::ny() const { return (*m_densities).ny(); }
  int Phantom::nz() const { return (*m_densities).nz(); }

  double Phantom::delta_x() const { return m_delta_x; }
  double Phantom::delta_y() const { return m_delta_y; }
  double Phantom::delta_z() const { return m_delta_z; }

  std::shared_ptr<const MatrixThreeTensor> Phantom::densities() const {
    return std::const_pointer_cast<const MatrixThreeTensor>(m_densities);
  }

  std::tuple<int,int,int> Phantom::index_at(const ThreeVector& position) const {
    auto normal = this->normal_coordinates(position);
    return std::make_tuple((int)std::get<0>(normal), (int)std::get<1>(normal), (int)std::get<2>(normal));
  }

  std::tuple<double,double,double> Phantom::normal_coordinates(const ThreeVector& position) const {
    return std::make_tuple(
      (position(0) - m_voxel_grid->v0()(0)) / m_delta_x,
      (position(1) - m_voxel_grid->v0()(1)) / m_delta_y,
      (position(2) - m_voxel_grid->v0()(2)) / m_delta_z
    );
  }

  const std::shared_ptr<const VoxelGrid>& Phantom::voxel_grid() const { return m_voxel_grid; }
  double Phantom::operator()(int xi, int yi, int zi) const { return (*m_densities)(xi, yi, zi); }
  const Compound& Phantom::compound(int xi, int yi, int zi) const { return *m_compounds[xi + yi*this->nx() + zi*this->nx()*this->ny()]; }
  std::shared_ptr<const Compound> Phantom::compound_ptr(int xi, int yi, int zi) const { return m_compounds[xi + yi*this->nx() + zi*this->nx()*this->ny()]; }

  bool Phantom::transport_photon_unitless_depth(Photon& photon, double depth) const {
    double current_depth = 0;
    double energy = photon.energy();
    bool inside = false;

    ThreeVector final_position = this->ray_trace_voxels(
      photon.position(), photon.momentum().three_momentum(),
      VoxelGrid::voxel_iterator(
        [&,this](double distance, int xi, int yi, int zi) -> double {
          double delta_depth = (*this)(xi, yi, zi) * this->compound(xi, yi, zi).photon_total_cross_section(energy) * distance;
          current_depth += delta_depth;
          if (current_depth < depth) return distance;

          inside = true;
          return (delta_depth - current_depth + depth) / (*this)(xi, yi, zi) / this->compound(xi, yi, zi).photon_total_cross_section(energy);
        }
      )
    );

    photon.position() = final_position;
    return inside;
  }

  ThreeVector Phantom::ray_trace_voxels(const ThreeVector& initial_position, const ThreeVector& direction, VoxelGrid::voxel_iterator it) const {
    return m_voxel_grid->ray_trace_voxels(initial_position, direction, it);
  }

  std::ofstream& Phantom::write(std::ofstream& os) const {
    m_voxel_grid->write(os);
    m_densities->write(os);
    return os;
  }

  std::ifstream& Phantom::read(std::ifstream& is) {
    {
      // Read into temporary voxel grid, then cast to const for local storage
      std::shared_ptr<VoxelGrid> new_grid = std::make_shared<VoxelGrid>();
      new_grid->read(is);
      m_voxel_grid = std::const_pointer_cast<const VoxelGrid>(new_grid);
    }
    m_densities->read(is);
    m_delta_x = (m_voxel_grid->vn()(0) - m_voxel_grid->v0()(0)) / this->nx();
    m_delta_y = (m_voxel_grid->vn()(1) - m_voxel_grid->v0()(1)) / this->ny();
    m_delta_z = (m_voxel_grid->vn()(2) - m_voxel_grid->v0()(2)) / this->nz();
    return is;
  }
};
