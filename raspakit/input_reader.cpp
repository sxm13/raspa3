module;

module input_reader;

import system;
import atom;
import component;
import simulationbox;
import forcefield;
import double3;
import units;
import print;
import sample_movies;
import threadpool;
import isotherm;
import multi_site_isotherm;
import pressure_range;

import <filesystem>;
import <fstream>;
import <cstdlib>;
import <iostream>;
import <sstream>;
import <exception>;
import <numbers>;
import <vector>;
import <array>;
import <complex>;
import <ios>;
import <optional>;

inline bool caseInSensStringCompare(const std::string& str1, const std::string& str2)
{
    return str1.size() == str2.size() && std::equal(str1.begin(), str1.end(), str2.begin(), [](auto a, auto b) {return std::tolower(a) == std::tolower(b); });
}

inline std::string trim(const std::string& s)
{
    auto start = s.begin();
    while (start != s.end() && std::isspace(*start)) {
        start++;
    }

    auto end = s.end();
    do {
        end--;
    } while (std::distance(start, end) > 0 && std::isspace(*end));

    return std::string(start, end + 1);
}

template<class T>
T parse(const std::string& arguments, [[maybe_unused]] const std::string& keyword, [[maybe_unused]] size_t lineNumber)
{
    T value;

    std::string str;
    std::istringstream ss(arguments);

    ss >> value;

    return value;
}

template<typename T>
std::vector<T> parseListOfSystemValues(const std::string& arguments, const std::string& keyword, size_t lineNumber)
{
    std::vector<T> list{};

    std::string str;
    std::istringstream ss(arguments);

    while (ss >> str)
    {
        if (trim(str).rfind("//", 0) == 0)
        {
            if (list.empty())
            {
                throw std::runtime_error(std::print("No values could be read for keyword '{}' at line: {}", keyword, lineNumber));
            }
            return list;
        }
        T value;
        std::istringstream s(str);
        if (s >> value)
        {
            list.push_back(value);
        }
        else
        {
            if (list.empty()) 
            {
                throw std::runtime_error(std::print("No values could be read for keyword '{}' at line: {}", keyword, lineNumber));
            }
            return list;
        }

    };

    if (list.empty()) 
    {
        throw std::runtime_error( std::print("No values could be read for keyword '{}' at line: {}", keyword, lineNumber));
    }
    return list;
}

int parseBoolean(const std::string& arguments, const std::string& keyword, size_t lineNumber)
{
    bool value{};

    std::istringstream ss(arguments);

    if (ss >> std::boolalpha >> value)
    {
        return value;
    };

    std::string str;
    std::istringstream ss2(arguments);
    if (ss2 >> str)
    {
        if (caseInSensStringCompare(str, "yes")) return true;
        if (caseInSensStringCompare(str, "no")) return false;
    };

    throw std::runtime_error(std::print("Numbers could not be read for keyword '{}' at line: {}", keyword, lineNumber));
}

template<>
std::vector<bool> parseListOfSystemValues(const std::string& arguments, const std::string& keyword, size_t lineNumber)
{
  std::vector<bool> list{};

  std::string str, boolString;
  std::istringstream ss(arguments);

  while (ss >> str)
  {
    if (trim(str).rfind("//", 0) == 0)
    {
      if (list.empty())
      {
          throw std::runtime_error(std::print("No values could be read for keyword '{}' at line: {}", keyword, lineNumber));
      }
      return list;
    }
    bool value = false;
    std::istringstream s(str);
    std::istringstream s2(str);
    if (s >> std::boolalpha >>  value)
    {
      list.push_back(value);
    }
    else if (s2 >> boolString)
    {
      if (caseInSensStringCompare(boolString, "yes")) 
      {
        list.push_back(true);
      }
      else if (caseInSensStringCompare(boolString, "no")) 
      {
        list.push_back(false);
      } 
      else
      {
        if (list.empty()) 
        {
          throw std::runtime_error(std::print("No values could be read for keyword '{}' at line: {}", keyword, lineNumber));
        }
        return list;
      }
    }
  };

  if (list.empty()) 
  {
    throw std::runtime_error( std::print("No values could be read for keyword '{}' at line: {}", keyword, lineNumber));
  }
  return list;
}


size_t parseInteger(const std::string& arguments, const std::string& keyword, size_t lineNumber)
{
  size_t value{};

  std::string str;
  std::istringstream ss(arguments);

  if (ss >> value)
  {
      return value;
  };

  throw std::runtime_error(std::print("Numbers could not be read for keyword '{}' at line: {}", keyword, lineNumber));
}

double parseDouble(const std::string& arguments, const std::string& keyword, size_t lineNumber)
{
  double value{};

  std::string str;
  std::istringstream ss(arguments);

  if (ss >> value)
  {
      return value;
  };

  throw std::runtime_error(std::print("Numbers could not be read for keyword '{}' at line: {}", keyword, lineNumber));
}

double3 parseDouble3(const std::string& arguments, const std::string& keyword, size_t lineNumber)
{
    double3 value{};

    std::string str;
    std::istringstream ss(arguments);

    if (ss >> value.x >> value.y >> value.z)
    {
        return value;
    };

    throw std::runtime_error(std::print("Numbers could not be read for keyword '{}' at line: {}", keyword, lineNumber));
}

std::string parseString(const std::string& arguments, const std::string& keyword, size_t lineNumber)
{
    std::string value{};

    std::string str;
    std::istringstream ss(arguments);

    if (ss >> value)
    {
        return value;
    };

    throw std::runtime_error(std::print("Numbers could not be read for keyword '{}' at line: {}", keyword, lineNumber));
}


InputReader::InputReader()
{
  char* env_p = std::getenv("RASPA_DIR");

  // environment variable 'RASPA_DIR' not set, assume current working directory
  if(!env_p)
  {
   // env_p = std::filesystem::current_path().c_str();
  }

  std::cout << "Path: " << std::filesystem::current_path() << std::endl;
  std::filesystem::path pathfile = std::filesystem::path(simulationSettingsFileName);
  if (!std::filesystem::exists(pathfile)) 
  {
    //pathfile = std::filesystem::path(env_p) / simulationSettingsFileName;
  }

  if (!std::filesystem::exists(pathfile)) throw std::runtime_error("Required file 'simulation.input' not found");

  std::ifstream fileInput{ pathfile };
  if (!fileInput) throw std::runtime_error("File 'simulation.input' exists, but error opening file");

  std::string line{};
  std::string keyword{};
  std::string arguments{};

  std::size_t numberOfSystems{ 0 };
  size_t lineNumber{ 0 };

  ForceField currentForceField;
 
  while (std::getline(fileInput, line))
  {
    lineNumber += 1;
    if (!line.empty())
    {
      std::istringstream iss(line);

      iss >> keyword;
      keyword = trim(keyword);
      std::getline(iss, arguments);           

      if (caseInSensStringCompare(keyword, "SimulationType"))
      {
        std::string str;
        std::istringstream ss(arguments);
        if (ss >> str)
        {
          if (caseInSensStringCompare(str, "MonteCarlo"))
          {
            simulationType = SimulationType::MonteCarlo;
            continue;
          }
          if (caseInSensStringCompare(str, "MolecularDynamics"))
          {
            simulationType = SimulationType::MolecularDynamics;
            continue;
          }
          if (caseInSensStringCompare(str, "Breakthrough"))
          {
            simulationType = SimulationType::Breakthrough;
            continue;
          }
          if (caseInSensStringCompare(str, "Minimization"))
          {
            simulationType = SimulationType::Minimization;
            continue;
          }
          if (caseInSensStringCompare(str, "MixturePrediction"))
          {
            simulationType = SimulationType::MixturePrediction;
            continue;
          }
          if (caseInSensStringCompare(str, "Fitting"))
          {
            simulationType = SimulationType::Fitting;
            continue;
          }
          if (caseInSensStringCompare(str, "Test"))
          {
            simulationType = SimulationType::Test;
            continue;
          }
        };
      }

      if (caseInSensStringCompare(keyword, std::string("NumberOfCycles")))
      {
        numberOfCycles = parse<size_t>(arguments, keyword, lineNumber);
        continue;
      }

      if (caseInSensStringCompare(keyword, std::string("NumberOfInitializationCycles")))
      {
        numberOfInitializationCycles = parse<size_t>(arguments, keyword, lineNumber);
        continue;
      }

      if (caseInSensStringCompare(keyword, std::string("NumberOfEquilibrationCycles")))
      {
        numberOfEquilibrationCycles = parse<size_t>(arguments, keyword, lineNumber);
        continue;
      }

      if (caseInSensStringCompare(keyword, std::string("PrintEvery")))
      {
        printEvery = parse<size_t>(arguments, keyword, lineNumber);
        continue;
      }

      if (caseInSensStringCompare(keyword, std::string("NumberOfBlocks")))
      {
        numberOfBlocks = parse<size_t>(arguments, keyword, lineNumber);
        continue;
      }

      if (caseInSensStringCompare(keyword, std::string("NumberOfThreads")))
      {
        numberOfThreads = parse<size_t>(arguments, keyword, lineNumber);
        continue;
      }

      if (caseInSensStringCompare(keyword, std::string("ThreadingType")))
      {
        std::string str;
        std::istringstream ss2(arguments);
        if (ss2 >> str)
        {
          if (caseInSensStringCompare(str, "Serial")) 
          {
            threadingType = ThreadPool::ThreadingType::Serial;
            continue;
          }
          if (caseInSensStringCompare(str, "ThreadPool")) 
          {
            threadingType = ThreadPool::ThreadingType::ThreadPool;
            continue;
          }
          if (caseInSensStringCompare(str, "OpenMP")) 
          {
            threadingType = ThreadPool::ThreadingType::OpenMP;
            continue;
          }
          if (caseInSensStringCompare(str, "GPU-Offload"))
          {
            threadingType = ThreadPool::ThreadingType::GPU_Offload;
            continue;
          }
        };
      }

      if (caseInSensStringCompare(keyword, std::string("Box")))
      {
        numberOfSystems += 1;
        systems.emplace_back(numberOfSystems - 1, ForceField(), std::vector<Component>{}, std::vector<size_t>{}, numberOfBlocks);
        systems.back().forceField = currentForceField;
        continue;
      }

      if (caseInSensStringCompare(keyword, std::string("ForceField")))
      {
        currentForceField = ForceField(pseudoAtomsFileName, forceFieldMixingRulesFileName, forceFieldOverwriteFileName);
        if (!systems.empty())
        {
          systems.back().forceField = currentForceField;
        }
        continue;
      }

      if (caseInSensStringCompare(keyword, "BoxLengths"))
      {
        requireExistingSystem(keyword, lineNumber);
        double3 value = parseDouble3(arguments, keyword, lineNumber);
        systems.back().simulationBox.setBoxLengths(value);
        continue;
      }

      if (caseInSensStringCompare(keyword, "BoxAngles"))
      {
        requireExistingSystem(keyword, lineNumber);
        double3 value = parseDouble3(arguments, keyword, lineNumber);
        systems.back().simulationBox.setBoxAngles((std::numbers::pi / 180.0) * value);
        continue;
      }

      if (caseInSensStringCompare(keyword, std::string("Framework")))
      {
        numberOfSystems += 1;
        systems.emplace_back(numberOfSystems - 1, ForceField(), std::vector<Component>{}, std::vector<size_t>{}, numberOfBlocks);
        systems.back().forceField = currentForceField;
        continue;
      }

      if (caseInSensStringCompare(keyword, std::string("FrameworkName")))
      {
        std::istringstream ss(arguments);
        std::string frameworkName;
        ss >> frameworkName;

        switch(simulationType)
        {
          case SimulationType::MonteCarlo:
          case SimulationType::MolecularDynamics:
          case SimulationType::Minimization:
          case SimulationType::Test:
          {

            systems.back().addComponent(Component(Component::Type::Framework, systems.back().components.size(), 
                                        systems.back().forceField, frameworkName, frameworkName, numberOfBlocks));
            continue;
            break;
          }
          case SimulationType::Breakthrough:
          case SimulationType::MixturePrediction:
          case SimulationType::Fitting:
          {
            systems.back().addComponent(Component(Component::Type::Framework, systems.back().components.size(), 
                                        ForceField(), frameworkName, std::nullopt, numberOfBlocks));
            continue;
            break;
          }
        }
      }

      if (caseInSensStringCompare(keyword, "ExternalTemperature"))
      {
        requireExistingSystem(keyword, lineNumber);
        double value = parseDouble(arguments, keyword, lineNumber);
        systems.back().simulationBox.temperature = value;
        systems.back().simulationBox.Beta = 1.0/(0.8314464919 * value);
        continue;
      }

      if (caseInSensStringCompare(keyword, "ExternalPressure"))
      {
        requireExistingSystem(keyword, lineNumber);
        double value = parseDouble(arguments, keyword, lineNumber);
        systems.back().simulationBox.input_pressure = value;
        systems.back().simulationBox.pressure = value / Units::PressureConversionFactor;
        continue;
      }

      if (caseInSensStringCompare(keyword, "ChargeMethod"))
      {
        requireExistingSystem(keyword, lineNumber);
        std::string value = parseString(arguments, keyword, lineNumber);
        if (caseInSensStringCompare(value, "None"))
        {
          systems.back().noCharges = true;
          continue;
        }
        if (caseInSensStringCompare(value, "Ewald")) 
        {
          systems.back().noCharges = false;
          continue;
        }
      }

      if (caseInSensStringCompare(keyword, "OmitEwaldFourier"))
      {
        requireExistingSystem(keyword, lineNumber);
        bool value = parseBoolean(arguments, keyword, lineNumber);
        systems.back().omitEwaldFourier = value;
        continue;
      }

      if (caseInSensStringCompare(keyword, "probabilityVolumeMove"))
      {
        requireExistingSystem(keyword, lineNumber);
        double value = parseDouble(arguments, keyword, lineNumber);
        systems.back().probabilityVolumeMove = value;
        continue;
      }

      if (caseInSensStringCompare(keyword, "Movies"))
      {
        //requireExistingSystem(keyword, lineNumber);
        //bool value = parseBoolean(arguments, keyword, lineNumber);
        //systems.back().sampleMovie.sample = value;
        continue;
      }

      if (caseInSensStringCompare(keyword, "WriteMoviesEvery"))
      {
        //requireExistingSystem(keyword, lineNumber);
        //size_t value = parseInteger(arguments, keyword, lineNumber);
        //systems.back().sampleMovie.writeEvery = value;
        continue;
      }

      if (caseInSensStringCompare(keyword, "PressureStart"))
      {
        requireExistingSystem(keyword, lineNumber);
        double value = parseDouble(arguments, keyword, lineNumber);
        systems.back().pressure_range.pressureStart = value;
        continue;
      }
      if (caseInSensStringCompare(keyword, "PressureEnd"))
      {
        requireExistingSystem(keyword, lineNumber);
        double value = parseDouble(arguments, keyword, lineNumber);
        systems.back().pressure_range.pressureEnd = value;
        continue;
      }
      if (caseInSensStringCompare(keyword, "NumberOfPressurePoints"))
      {
        requireExistingSystem(keyword, lineNumber);
        size_t value = parse<size_t>(arguments, keyword, lineNumber);
        systems.back().pressure_range.numberOfPoints = value;
        continue;
      }
      if (caseInSensStringCompare(keyword, "PressureScale"))
      {
        requireExistingSystem(keyword, lineNumber);
        std::string str;
        std::istringstream ss(arguments);
        if (ss >> str)
        {
          if (caseInSensStringCompare(str, "Log"))
          {
            systems.back().pressure_range.scale = PressureRange::Scale::Log;
            continue;
          }
          if (caseInSensStringCompare(str, "Linear"))
          {
            systems.back().pressure_range.scale = PressureRange::Scale::Linear;
            continue;
          }
        }
      }

      if (caseInSensStringCompare(keyword, "ColumnVoidFraction"))
      {
        requireExistingSystem(keyword, lineNumber);
        double value = parseDouble(arguments, keyword, lineNumber);
        systems.back().columnVoidFraction = value;
        continue;
      }
      if (caseInSensStringCompare(keyword, "ParticleDensity"))
      {
        requireExistingSystem(keyword, lineNumber);
        double value = parseDouble(arguments, keyword, lineNumber);
        systems.back().columnParticleDensity = value;
        continue;
      }
      if (caseInSensStringCompare(keyword, "TotalPressure"))
      {
        requireExistingSystem(keyword, lineNumber);
        double value = parseDouble(arguments, keyword, lineNumber);
        systems.back().columnTotalPressure = value;
        continue;
      }
      if (caseInSensStringCompare(keyword, "PressureGradient"))
      {
        requireExistingSystem(keyword, lineNumber);
        double value = parseDouble(arguments, keyword, lineNumber);
        systems.back().columnPressureGradient = value;
        continue;
      }
      if (caseInSensStringCompare(keyword, "ColumnEntranceVelocity"))
      {
        requireExistingSystem(keyword, lineNumber);
        double value = parseDouble(arguments, keyword, lineNumber);
        systems.back().columnEntranceVelocity = value;
        continue;
      }
      if (caseInSensStringCompare(keyword, "TimeStep"))
      {
        requireExistingSystem(keyword, lineNumber);
        double value = parseDouble(arguments, keyword, lineNumber);
        systems.back().columnTimeStep = value;
        continue;
      }
      if (caseInSensStringCompare(keyword, "NumberOfTimeSteps"))
      {
        requireExistingSystem(keyword, lineNumber);
        std::string str;
        std::istringstream ss(arguments);
        if (ss >> str)
        {
          if (caseInSensStringCompare(str, "auto"))
          {
            systems.back().columnAutoNumberOfTimeSteps = true;
          }
          else
          {
            size_t value = parse<size_t>(arguments, keyword, lineNumber);
            systems.back().columnNumberOfTimeSteps = value;
            systems.back().columnAutoNumberOfTimeSteps = false;
          }
          continue;
        }
      }
      if (caseInSensStringCompare(keyword, "WriteEvery"))
      {
        size_t value = parse<size_t>(arguments, keyword, lineNumber);
        this->writeEvery = value;
        continue;
      }
      if (caseInSensStringCompare(keyword, "ColumnLength"))
      {
        requireExistingSystem(keyword, lineNumber);
        double value = parseDouble(arguments, keyword, lineNumber);
        systems.back().columnLength = value;
        continue;
      }
      if (caseInSensStringCompare(keyword, "NumberOfGridPoints"))
      {
        requireExistingSystem(keyword, lineNumber);
        size_t value = parse<size_t>(arguments, keyword, lineNumber);
        systems.back().columnNumberOfGridPoints = value;
        continue;
      }

      if (caseInSensStringCompare(keyword, "MixturePredictionMethod"))
      {
        requireExistingSystem(keyword, lineNumber);
        std::string str;
        std::istringstream ss(arguments);
        if (ss >> str)
        {
          if (caseInSensStringCompare(str, "IAST")) 
          {
            systems.back().mixturePredictionMethod = MultiSiteIsotherm::PredictionMethod::IAST;
            continue;
          }
          if (caseInSensStringCompare(str, "SIAST")) 
          {
            systems.back().mixturePredictionMethod = MultiSiteIsotherm::PredictionMethod::SIAST;
            continue;
          }
          if (caseInSensStringCompare(str, "EI")) 
          {
            systems.back().mixturePredictionMethod = MultiSiteIsotherm::PredictionMethod::EI;
            continue;
          }
          if (caseInSensStringCompare(str, "SEI")) 
          {
            systems.back().mixturePredictionMethod = MultiSiteIsotherm::PredictionMethod::SEI;
            continue;
          }
        };
      }

      if (caseInSensStringCompare(keyword, std::string("Component")))
      {
        requireExistingSystem(keyword, lineNumber);
        
        std::istringstream ss(arguments);
        std::string c, moleculeNameKeyword,remainder;
        ss >> c >> moleculeNameKeyword;
        std::getline(ss, remainder);

        std::vector<std::string> values = parseListOfSystemValues<std::string>(remainder, keyword, lineNumber);
        values.resize(systems.size(), values.back());
        for (size_t i = 0; i < systems.size(); ++i)
        {
          switch(simulationType)
          {
            case SimulationType::MonteCarlo:
            case SimulationType::MolecularDynamics:
            case SimulationType::Minimization:
            case SimulationType::Test:
              systems[i].addComponent(Component(Component::Type::Adsorbate, systems[i].components.size(),
                    systems[numberOfSystems - 1].forceField, values[i], values[i], numberOfBlocks));
              break;
            case SimulationType::Breakthrough:
            case SimulationType::MixturePrediction:
            case SimulationType::Fitting:
              systems[i].addComponent(Component(Component::Type::Adsorbate, systems[i].components.size(),
                    ForceField(), values[i], std::nullopt, numberOfBlocks));
              break;
          }
        }
        continue;
      }

      
      if (caseInSensStringCompare(keyword, "CreateNumberOfMolecules"))
      {
        requireExistingSystemAndComponent(keyword, lineNumber);

        std::vector<size_t> values = parseListOfSystemValues<size_t>(arguments, keyword, lineNumber);
        values.resize(systems.size(), values.back());
        for (size_t i = 0; i < systems.size(); ++i)
        {
          systems[i].components.back().initialNumberOfMolecules = values[i];
        }
        continue;
      }

      if (caseInSensStringCompare(keyword, "MolFraction"))
      {
        requireExistingSystem(keyword, lineNumber);

        std::vector<double> values = parseListOfSystemValues<double>(arguments, keyword, lineNumber);
    
        values.resize(systems.size(), values.back());
        for (size_t i = 0; i < systems.size(); ++i)
        {
          systems[i].components.back().molFraction = values[i];
        }
        continue;
      }
      if (caseInSensStringCompare(keyword, "FileName"))
      {
        std::string str;
        std::istringstream ss(arguments);
        if (ss >> str)
        {
          for (size_t i = 0; i < systems.size(); ++i)
          {
            systems[i].components.back().filename = str;
          }
          continue;
        }
      }
      //if (caseInSensStringCompare(keyword, "PressureScale"))
      //{
      //  std::istringstream ss(arguments);
      //  std::vector<std::string> values = parseListOfSystemValues<std::string>(arguments, keyword, lineNumber);
      //  values.resize(systems.size(), values.back());
      //  for (size_t i = 0; i < systems.size(); ++i)
      //  {
      //      // FIX!! no components exists yet
      //    //if (caseInSensStringCompare(values[i], "Log")) systems[i].components.back().pressureScale = Component::PressureScale::Log;
      //    //if (caseInSensStringCompare(values[i], "Linear")) systems[i].components.back().pressureScale = Component::PressureScale::Normal;
      //    //if (caseInSensStringCompare(values[i], "Normal")) systems[i].components.back().pressureScale = Component::PressureScale::Normal;
      //  }
      //  continue;
      //}
      if (caseInSensStringCompare(keyword, "CarrierGas"))
      {
        requireExistingSystem(keyword, lineNumber);

        std::vector<bool> values = parseListOfSystemValues<bool>(arguments, keyword, lineNumber);
    
        values.resize(systems.size(), values.back());
        for (size_t i = 0; i < systems.size(); ++i)
        {
          systems[i].components.back().isCarrierGas = values[i];
        }
        continue;
      }
      if (caseInSensStringCompare(keyword, "MassTransferCoefficient"))
      {
        requireExistingSystem(keyword, lineNumber);

        std::vector<double> values = parseListOfSystemValues<double>(arguments, keyword, lineNumber);
    
        values.resize(systems.size(), values.back());
        for (size_t i = 0; i < systems.size(); ++i)
        {
          systems[i].components.back().massTransferCoefficient = values[i];
        }
        continue;
      }
      if (caseInSensStringCompare(keyword, "AxialDispersionCoefficient"))
      {
        requireExistingSystem(keyword, lineNumber);

        std::vector<double> values = parseListOfSystemValues<double>(arguments, keyword, lineNumber);
    
        values.resize(systems.size(), values.back());
        for (size_t i = 0; i < systems.size(); ++i)
        {
          systems[i].components.back().axialDispersionCoefficient = values[i];
        }
        continue;
      }


      if (caseInSensStringCompare(keyword, "TranslationProbability"))
      {
        requireExistingSystemAndComponent(keyword, lineNumber);
        std::vector<double> values = parseListOfSystemValues<double>(arguments, keyword, lineNumber);
    
        values.resize(systems.size(), values.back());
        for (size_t i = 0; i < systems.size(); ++i)
        {
          systems[i].components.back().probabilityTranslationMove = values[i];
        }
        continue;
      }

      if (caseInSensStringCompare(keyword, "RandomTranslationProbability"))
      {
        requireExistingSystemAndComponent(keyword, lineNumber);
        std::vector<double> values = parseListOfSystemValues<double>(arguments, keyword, lineNumber);
    
        values.resize(systems.size(), values.back());
        for (size_t i = 0; i < systems.size(); ++i)
        {
          systems[i].components.back().probabilityRandomTranslationMove = values[i];
        }
        continue;
      }

      if (caseInSensStringCompare(keyword, "RotationProbability"))
      {
        requireExistingSystemAndComponent(keyword, lineNumber);
        std::vector<double> values = parseListOfSystemValues<double>(arguments, keyword, lineNumber);

        values.resize(systems.size(), values.back());
        for (size_t i = 0; i < systems.size(); ++i)
        {
          systems[i].components.back().probabilityRotationMove = values[i];
        }
        continue;
      }

      if (caseInSensStringCompare(keyword, "RandomRotationProbability"))
      {
        requireExistingSystemAndComponent(keyword, lineNumber);
        std::vector<double> values = parseListOfSystemValues<double>(arguments, keyword, lineNumber);

        values.resize(systems.size(), values.back());
        for (size_t i = 0; i < systems.size(); ++i)
        {
          systems[i].components.back().probabilityRandomRotationMove = values[i];
        }
        continue;
      }

      if (caseInSensStringCompare(keyword, "ReinsertionProbability"))
      {
        requireExistingSystemAndComponent(keyword, lineNumber);
        std::vector<double> values = parseListOfSystemValues<double>(arguments, keyword, lineNumber);

        values.resize(systems.size(), values.back());
        for (size_t i = 0; i < systems.size(); ++i)
        {
          systems[i].components.back().probabilityReinsertionMove_CBMC = values[i];
        }
        continue;
      }

      if (caseInSensStringCompare(keyword, "SwapProbability"))
      {
        requireExistingSystemAndComponent(keyword, lineNumber);
        std::vector<double> values = parseListOfSystemValues<double>(arguments, keyword, lineNumber);

        values.resize(systems.size(), values.back());
        for (size_t i = 0; i < systems.size(); ++i)
        {
          systems[i].components.back().probabilitySwapMove_CBMC = values[i];
        }
        continue;
      }

      if (caseInSensStringCompare(keyword, "GibbsSwapProbability"))
      {
        requireExistingSystemAndComponent(keyword, lineNumber);
        std::vector<double> values = parseListOfSystemValues<double>(arguments, keyword, lineNumber);

        values.resize(systems.size(), values.back());
        for (size_t i = 0; i < systems.size(); ++i)
        {
          systems[i].components.back().probabilityGibbsSwapMove_CBMC = values[i];
        }
        continue;
      }

      
      if (caseInSensStringCompare(keyword, "CFCMC_CBMC_SwapProbability"))
      {
        requireExistingSystemAndComponent(keyword, lineNumber);
        std::vector<double> values = parseListOfSystemValues<double>(arguments, keyword, lineNumber);

        values.resize(systems.size(), values.back());
        for (size_t i = 0; i < systems.size(); ++i)
        {
          systems[i].components.back().probabilitySwapMove_CFCMC_CBMC = values[i];
        }
        continue;
      }

      if (caseInSensStringCompare(keyword, "WidomProbability"))
      {
        requireExistingSystemAndComponent(keyword, lineNumber);
        std::vector<double> values = parseListOfSystemValues<double>(arguments, keyword, lineNumber);

        values.resize(systems.size(), values.back());
        for (size_t i = 0; i < systems.size(); ++i)
        {
          systems[i].components.back().probabilityWidomMove = values[i];
        }
        continue;
      }

      if (caseInSensStringCompare(keyword, "CFCMCWidomProbability"))
      {
        requireExistingSystemAndComponent(keyword, lineNumber);
        std::vector<double> values = parseListOfSystemValues<double>(arguments, keyword, lineNumber);

        values.resize(systems.size(), values.back());
        for (size_t i = 0; i < systems.size(); ++i)
        {
          systems[i].components.back().probabilityWidomMove_CFCMC = values[i];
        }
        continue;
      }

      if (caseInSensStringCompare(keyword, "CBCFCMCWidomProbability"))
      {
        requireExistingSystemAndComponent(keyword, lineNumber);
        std::vector<double> values = parseListOfSystemValues<double>(arguments, keyword, lineNumber);

        values.resize(systems.size(), values.back());
        for (size_t i = 0; i < systems.size(); ++i)
        {
          systems[i].components.back().probabilityWidomMove_CFCMC_CBMC = values[i];
        }
        continue;
      }

      if (caseInSensStringCompare(keyword, "MassTransferCoefficient"))
      {
        requireExistingSystemAndComponent(keyword, lineNumber);
        std::vector<double> values = parseListOfSystemValues<double>(arguments, keyword, lineNumber);

        values.resize(systems.size(), values.back());
        for (size_t i = 0; i < systems.size(); ++i)
        {
          systems[i].components.back().massTransferCoefficient = values[i];
        }
        continue;
      }
      if (caseInSensStringCompare(keyword, "AxialDispersionCoefficient"))
      {
        requireExistingSystemAndComponent(keyword, lineNumber);
        std::vector<double> values = parseListOfSystemValues<double>(arguments, keyword, lineNumber);

        values.resize(systems.size(), values.back());
        for (size_t i = 0; i < systems.size(); ++i)
        {
          systems[i].components.back().axialDispersionCoefficient = values[i];
        }
        continue;
      }

      if (caseInSensStringCompare(keyword, "NumberOfIsothermSites"))
      {
        size_t value = parse<size_t>(arguments, keyword, lineNumber);
        for (size_t i = 0; i < systems.size(); ++i)
        {
          systems[i].components.back().isotherm.numberOfSites = value;
        }
        continue;
      }
      if (caseInSensStringCompare(keyword, "Langmuir"))
      {
        std::vector<double> values = parseListOfSystemValues<double>(arguments, keyword, lineNumber);
        if(values.size() < 2)
        {
          throw std::runtime_error("Error: Langmuir requires two parameters");
        }
        values.resize(2);
        const Isotherm isotherm = Isotherm(Isotherm::Type::Langmuir, values, 2);
        for (size_t i = 0; i < systems.size(); ++i)
        {
          systems[i].components.back().isotherm.add(isotherm);
        }
        continue;
      }
      if (caseInSensStringCompare(keyword, "Anti-Langmuir"))
      {
        std::vector<double> values = parseListOfSystemValues<double>(arguments, keyword, lineNumber);
        if(values.size() < 2)
        {
          throw std::runtime_error("Error: Anti-Langmuir requires two parameters");
        }
        values.resize(2);
        const Isotherm isotherm = Isotherm(Isotherm::Type::Anti_Langmuir, values, 2);
        for (size_t i = 0; i < systems.size(); ++i)
        {
          systems[i].components.back().isotherm.add(isotherm);
        }
        continue;
      }
      if (caseInSensStringCompare(keyword, "BET"))
      {
        std::vector<double> values = parseListOfSystemValues<double>(arguments, keyword, lineNumber);
        if(values.size() < 3)
        {
          throw std::runtime_error("Error: BET requires three parameters");
        }
        values.resize(3);
        const Isotherm isotherm = Isotherm(Isotherm::Type::BET, values, 3);
        for (size_t i = 0; i < systems.size(); ++i)
        {
          systems[i].components.back().isotherm.add(isotherm);
        }
        continue;
      }
      if (caseInSensStringCompare(keyword, "Henry"))
      {
        std::vector<double> values = parseListOfSystemValues<double>(arguments, keyword, lineNumber);
        if(values.size() < 1)
        {
          throw std::runtime_error("Error: Henry requires one parameter");
        }
        values.resize(1);
        const Isotherm isotherm = Isotherm(Isotherm::Type::Henry, values, 1);
        for (size_t i = 0; i < systems.size(); ++i)
        {
          systems[i].components.back().isotherm.add(isotherm);
        }
        continue;
      }
      if (caseInSensStringCompare(keyword, "Freundlich"))
      {
        std::vector<double> values = parseListOfSystemValues<double>(arguments, keyword, lineNumber);
        if(values.size() < 2)
        {
          throw std::runtime_error("Error: Freundlich requires two parameters");
        }
        values.resize(2);
        const Isotherm isotherm = Isotherm(Isotherm::Type::Freundlich, values, 2);
        for (size_t i = 0; i < systems.size(); ++i)
        {
          systems[i].components.back().isotherm.add(isotherm);
        }
        continue;
      }
      if (caseInSensStringCompare(keyword, "Sips"))
      {
        std::vector<double> values = parseListOfSystemValues<double>(arguments, keyword, lineNumber);
        if(values.size() < 3)
        {
          throw std::runtime_error("Error: Sips requires three parameters");
        }
        values.resize(3);
        const Isotherm isotherm = Isotherm(Isotherm::Type::Sips, values, 3);
        for (size_t i = 0; i < systems.size(); ++i)
        {
          systems[i].components.back().isotherm.add(isotherm);
        }
        continue;
      }
      if (caseInSensStringCompare(keyword, "Langmuir-Freundlich"))
      {
        std::vector<double> values = parseListOfSystemValues<double>(arguments, keyword, lineNumber);
        if(values.size() < 3)
        {
          throw std::runtime_error("Error: Langmuir requires three parameters");
        }
        values.resize(3);
        const Isotherm isotherm = Isotherm(Isotherm::Type::Langmuir_Freundlich, values, 3);
        for (size_t i = 0; i < systems.size(); ++i)
        {
          systems[i].components.back().isotherm.add(isotherm);
        }
        continue;
      }
      if (caseInSensStringCompare(keyword, "Redlich-Peterson"))
      {
        std::vector<double> values = parseListOfSystemValues<double>(arguments, keyword, lineNumber);
        if(values.size() < 3)
        {
          throw std::runtime_error("Error: Redlich-Peterson requires three parameters");
        }
        values.resize(3);
        const Isotherm isotherm = Isotherm(Isotherm::Type::Redlich_Peterson, values, 3);
        for (size_t i = 0; i < systems.size(); ++i)
        {
          systems[i].components.back().isotherm.add(isotherm);
        }
        continue;
      }
      if (caseInSensStringCompare(keyword, "Toth"))
      {
        std::vector<double> values = parseListOfSystemValues<double>(arguments, keyword, lineNumber);
        if(values.size() < 3)
        {
          throw std::runtime_error("Error: Toth requires three parameters");
        }
        values.resize(3);
        const Isotherm isotherm = Isotherm(Isotherm::Type::Toth, values, 3);
        for (size_t i = 0; i < systems.size(); ++i)
        {
          systems[i].components.back().isotherm.add(isotherm);
        }
        continue;
      }
      if (caseInSensStringCompare(keyword, "Unilan"))
      {
        std::vector<double> values = parseListOfSystemValues<double>(arguments, keyword, lineNumber);
        if(values.size() < 3)
        {
          throw std::runtime_error("Error: Unilan requires three parameters");
        }
        values.resize(3);
        const Isotherm isotherm = Isotherm(Isotherm::Type::Unilan, values, 3);
        for (size_t i = 0; i < systems.size(); ++i)
        {
          systems[i].components.back().isotherm.add(isotherm);
        }
        continue;
      }
      if (caseInSensStringCompare(keyword, "O'Brien&Myers"))
      {
        std::vector<double> values = parseListOfSystemValues<double>(arguments, keyword, lineNumber);
        if(values.size() < 3)
        {
          throw std::runtime_error("Error: O'Brien&Myers requires three parameters");
        }
        values.resize(3);
        const Isotherm isotherm = Isotherm(Isotherm::Type::OBrien_Myers, values, 3);
        for (size_t i = 0; i < systems.size(); ++i)
        {
          systems[i].components.back().isotherm.add(isotherm);
        }
        continue;
      }
      if (caseInSensStringCompare(keyword, "Quadratic"))
      {
        std::vector<double> values = parseListOfSystemValues<double>(arguments, keyword, lineNumber);
        if(values.size() < 3)
        {
          throw std::runtime_error("Error: Quadratic requires three parameters");
        }
        values.resize(3);
        const Isotherm isotherm = Isotherm(Isotherm::Type::Quadratic, values, 3);
        for (size_t i = 0; i < systems.size(); ++i)
        {
          systems[i].components.back().isotherm.add(isotherm);
        }
        continue;
      }
      if (caseInSensStringCompare(keyword, "Temkin"))
      {
        std::vector<double> values = parseListOfSystemValues<double>(arguments, keyword, lineNumber);
        if(values.size() < 3)
        {
          throw std::runtime_error("Error: Temkin requires three parameters");
        }
        values.resize(3);
        const Isotherm isotherm = Isotherm(Isotherm::Type::Temkin, values, 3);
        for (size_t i = 0; i < systems.size(); ++i)
        {
          systems[i].components.back().isotherm.add(isotherm);
        }
        continue;
      }
      if (caseInSensStringCompare(keyword, "ColumnPressure"))
      {
        std::vector<size_t> values = parseListOfSystemValues<size_t>(arguments, keyword, lineNumber);
        values.resize(systems.size(), values.back());
        for (size_t i = 0; i < systems.size(); ++i)
        {
          systems[i].components.back().columnPressure = values[i];
        }
        continue;
      }
      if (caseInSensStringCompare(keyword, "ColumnLoading"))
      {
        std::vector<size_t> values = parseListOfSystemValues<size_t>(arguments, keyword, lineNumber);
        values.resize(systems.size(), values.back());
        for (size_t i = 0; i < systems.size(); ++i)
        {
          systems[i].components.back().columnLoading = values[i];
        }
        continue;
      }

      if (caseInSensStringCompare(keyword, "ColumnError"))
      {
        std::vector<size_t> values = parseListOfSystemValues<size_t>(arguments, keyword, lineNumber);
        values.resize(systems.size(), values.back());
        for (size_t i = 0; i < systems.size(); ++i)
        {
          systems[i].components.back().columnError = values[i];
        }
        continue;
      }

      if (!(keyword.starts_with("//") || (keyword.starts_with("#"))))
      {
          throw std::runtime_error(std::print("Error [Input]: unrecognized keyword '{}' at line: {}", keyword, lineNumber));
      }

    }

  }

  // Post-compute
  // ========================================================
  
  for (size_t i = 0; i < systems.size(); ++i)
  {
    systems[i].maxIsothermTerms = 0;
    if(!systems[i].components.empty())
    {
      std::vector<Component>::iterator maxIsothermTermsIterator = 
            std::max_element(systems[i].components.begin(), systems[i].components.end(),
              [] (Component& lhs, Component& rhs) {
                  return lhs.isotherm.numberOfSites < rhs.isotherm.numberOfSites;
              });
      systems[i].maxIsothermTerms = maxIsothermTermsIterator->isotherm.numberOfSites;
    }
  }

  // Checks
  // ========================================================
    
  for (size_t i = 0; i < systems.size(); ++i)
  {
    double sum = 0.0;
    for(size_t j = 0; j < systems[i].components.size(); ++j)
    {
      if(systems[i].components[j].type != Component::Type::Framework)
      {
        sum += systems[i].components[j].molFraction;
      }
    }
    if(std::abs(sum-1.0)>1e-15)
    {
      std::cout << "Normalizing: Gas-phase molfractions did not sum exactly to unity!\n\n";
      for(size_t j = 0; j < systems[i].components.size(); ++j)
      {
        if(systems[i].components[j].type != Component::Type::Framework)
        {
          systems[i].components[j].molFraction /= sum;
        }
      }
    }
  }

  for (size_t i = 0; i < systems.size(); ++i)
  {
    systems[i].numberOfCarrierGases = 0;
    systems[i].carrierGasComponent = 0;
    for(size_t j = 0; j < systems[i].components.size(); ++j)
    {
      if(systems[i].components[j].type != Component::Type::Framework)
      {
        if(systems[i].components[j].isCarrierGas)
        {
          systems[i].carrierGasComponent = j;
          std::vector<double> values{1.0, 0.0};
          const Isotherm isotherm = Isotherm(Isotherm::Type::Langmuir, values, 2);
          systems[i].components[systems[i].carrierGasComponent].isotherm.add(isotherm);
          systems[i].components[systems[i].carrierGasComponent].isotherm.numberOfSites = 1;

          systems[i].numberOfCarrierGases++;
        }
      }
    }

    if(simulationType == SimulationType::Breakthrough)
    {
      if(systems[i].numberOfCarrierGases == 0)
      {
        throw std::runtime_error("Error [Breakthrough]: no carrier gas component present");
      }
      if(systems[i].numberOfCarrierGases > 1)
      {
        throw std::runtime_error("Error [Breakthrough]: multiple carrier gas component present (there can be only one)");
      }
    }
  }

}

void InputReader::requireExistingSystem(const std::string& keyword, size_t lineNumber)
{
    if (systems.empty()) {
        throw std::runtime_error(std::print("No system (Framework or Box) defined yet at keyword '{}' at line: {}", keyword, lineNumber));
    }
}

void InputReader::requireExistingSystemAndComponent(const std:: string &keyword, size_t lineNumber)
{
    if (systems.empty()) {
        throw std::runtime_error(
            std::print("No system (Framework or Box) defined yet at keyword '{}' at line: {}", keyword, lineNumber)
            );
    }
    if (systems[0].components.empty()) {
        throw std::runtime_error(
            std::print("No component defined yet at keyword '{}' at line: {}", keyword, lineNumber)
            );
    }
}
