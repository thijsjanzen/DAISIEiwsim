#pragma once

struct island {
  virtual void update_area(double t) = 0;
  double area;
};

struct island_static : island {
  void update_area(double t) override {
    // static nothing happens
    return;
  }
  
  double area = 1.0;
};

struct island_beta : island{
public:
  island_beta(double max_area,
              double total_island_age,
              double proportional_peak_t) : 
                  Amax(max_area),
                  proptime_max(proportional_peak_t),
                  Tmax(total_island_age),
                  f(proptime_max / (1 - proptime_max)){
  }
  
  double area = 1.0;
  
  void update_area(double t) override {
    double proptime = t / Tmax;
    double a =  f * peak / (1 + f);
    double b = peak / (1 + f); 
    
    double divisor = std::pow(a / (a + b), a) * std::pow((b / (a + b)), b);
    double x = std::pow(1 - proptime, b) / divisor;
    area = Amax * std::pow(proptime, a) * x;
  }
private:
  const double proptime_max;
  const double f;
  double peak;
  const double Amax;
  const double Tmax;
};

struct island_angular : island {
public:
  island_angular(double sea_level_frequency,
                 double total_island_age,
                 double total_time,
                 double amplitude,
                 double current_area,
                 double island_gradient_angle) : 
  angular_freq(2 * M_PI * sea_level_frequency),
  Tmax(total_island_age),
  proptime_curr(total_time / total_island_age),
  ampl(amplitude),
  Acurr(current_area),
  theta(island_gradient_angle) {
  }
  
  double area = 1.0;
  
  void update_area(double t) override {
    double proptime = t / Tmax;
    auto delta_sl = ampl * std::cos((proptime_curr - proptime) * angular_freq);
    auto r_curr = std::sqrt((Acurr) / M_PI);
    auto h_curr = std::tan(theta) * r_curr;
    auto h_delta = h_curr - ampl + delta_sl;
    if (h_delta < 0) h_delta = 0.0;
    auto x = h_delta / std::tan(theta);
    area = M_PI * x * x;
  }
private:
  const double angular_freq;
  const double proptime_curr;
  const double ampl;
  const double Acurr;
  const double theta;
  const double Tmax;
};


struct island_beta_angular : island {
public:
  island_beta_angular(double max_area,
              double total_island_age,
              double proportional_peak_t) : 
  Amax(max_area),
  proptime_max(proportional_peak_t),
  Tmax(total_island_age),
  f(proptime_max / (1 - proptime_max)){
  }
  
  double area = 1.0;
  
  void update_area(double t) override {
    double proptime = t / Tmax;
    double a =  f * peak / (1 + f);
    double b = peak / (1 + f); 
    
    double divisor = std::pow(a / (a + b), a) * std::pow((b / (a + b)), b);
    double x = std::pow(1 - proptime, b) / divisor;
    area = Amax * std::pow(proptime, a) * x;
  }
private:
  const double proptime_max;
  const double f;
  double peak;
  const double Amax;
  const double Tmax;
};

