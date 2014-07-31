#pragma once

#include "util/common.h"
#include "proto/neural_network.pb.h"
#include "base/matrix.h"
#include "neural_network/parameter.h"
#include "neural_network/activation_function_inl.h"

namespace PS {
namespace NN {

template<typename V> class Net;
template<typename V> class Layer;
template<typename V> using LayerPtr = std::shared_ptr<Layer<V>>;
template<typename V> using LayerPtrList = std::vector<LayerPtr<V>>;

template<typename V> class Layer {
 public:
  Layer() { }
  void set(const LayerConfig& config) { cf_ = config; }

  size_t size() { return cf_.size(); }

  // all in layers and out layers have been added
  virtual void initModel() = 0;
  virtual V forward() = 0;
  virtual void backward() = 0;

  void addInLayer(const LayerPtr<V>& in) {
    CHECK_EQ(name(), in->out_layers_.back()->name());
    in_args_.push_back(in->out_args_.back());
    in_layers_.push_back(in);
  }

  void addOutLayer(const LayerPtr<V>& out) {
    out_layers_.push_back(out);
    ParameterPtr<V> arg(new Parameter<V>());
    arg->value = MatrixPtr<V>(new DenseMatrix<V>());
    if (!cf_.type() == LayerConfig::DATA) {
      arg->gradient = MatrixPtr<V>(new DenseMatrix<V>());
    }
    out_args_.push_back(arg);
  }

  string name() { return cf_.name(); }
  ParameterPtr<V>& model() { return model_; }
  const LayerConfig& config() { return cf_; }

 protected:
  LayerConfig cf_;
  LayerPtrList<V> in_layers_, out_layers_;
  ParameterPtrList<V> in_args_, out_args_;
  ActivationFuncPtr<V> activation_;
  ParameterPtr<V> model_;

 private:
  DISALLOW_COPY_AND_ASSIGN(Layer);
};

} // namespace NN
} // namespace PS
