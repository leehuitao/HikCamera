/***********************************************************
 * File generated by the HALCON-Compiler hcomp version 18.11
 * Usage: Interface to C++
 *
 * Software by: MVTec Software GmbH, www.mvtec.com
 ***********************************************************/


#ifndef HCPP_HSHEETOFLIGHTMODEL
#define HCPP_HSHEETOFLIGHTMODEL

namespace HalconCpp
{

// Represents an instance of the data structure required to perform 3D measurements with the sheet-of-light technique.
class LIntExport HSheetOfLightModel : public HHandle
{

public:

  // Create an uninitialized instance
  HSheetOfLightModel():HHandle() {}

  // Copy constructor
  HSheetOfLightModel(const HSheetOfLightModel& source) : HHandle(source) {}

  // Copy constructor
  HSheetOfLightModel(const HHandle& handle);

  // Create HSheetOfLightModel from handle, taking ownership
  explicit HSheetOfLightModel(Hlong handle);

  bool operator==(const HHandle& obj) const
  {
    return HHandleBase::operator==(obj);
  }

  bool operator!=(const HHandle& obj) const
  {
    return HHandleBase::operator!=(obj);
  }

protected:

  // Verify matching semantic type ('sheet_of_light_model')!
  virtual void AssertType(Hphandle handle) const;

public:

  // Deep copy of all data represented by this object instance
  HSheetOfLightModel Clone() const;



/*****************************************************************************
 * Operator-based class constructors
 *****************************************************************************/

  // read_sheet_of_light_model: Read a sheet-of-light model from a file and create a new model.
  explicit HSheetOfLightModel(const HString& FileName);

  // read_sheet_of_light_model: Read a sheet-of-light model from a file and create a new model.
  explicit HSheetOfLightModel(const char* FileName);

#ifdef _WIN32
  // read_sheet_of_light_model: Read a sheet-of-light model from a file and create a new model.
  explicit HSheetOfLightModel(const wchar_t* FileName);
#endif

  // create_sheet_of_light_model: Create a model to perform 3D-measurements using the sheet-of-light technique.
  explicit HSheetOfLightModel(const HRegion& ProfileRegion, const HTuple& GenParamName, const HTuple& GenParamValue);

  // create_sheet_of_light_model: Create a model to perform 3D-measurements using the sheet-of-light technique.
  explicit HSheetOfLightModel(const HRegion& ProfileRegion, const HString& GenParamName, Hlong GenParamValue);

  // create_sheet_of_light_model: Create a model to perform 3D-measurements using the sheet-of-light technique.
  explicit HSheetOfLightModel(const HRegion& ProfileRegion, const char* GenParamName, Hlong GenParamValue);

#ifdef _WIN32
  // create_sheet_of_light_model: Create a model to perform 3D-measurements using the sheet-of-light technique.
  explicit HSheetOfLightModel(const HRegion& ProfileRegion, const wchar_t* GenParamName, Hlong GenParamValue);
#endif




  /***************************************************************************
   * Operators                                                               *
   ***************************************************************************/

  // Read a sheet-of-light model from a file and create a new model.
  void ReadSheetOfLightModel(const HString& FileName);

  // Read a sheet-of-light model from a file and create a new model.
  void ReadSheetOfLightModel(const char* FileName);

#ifdef _WIN32
  // Read a sheet-of-light model from a file and create a new model.
  void ReadSheetOfLightModel(const wchar_t* FileName);
#endif

  // Write a sheet-of-light model to a file.
  void WriteSheetOfLightModel(const HString& FileName) const;

  // Write a sheet-of-light model to a file.
  void WriteSheetOfLightModel(const char* FileName) const;

#ifdef _WIN32
  // Write a sheet-of-light model to a file.
  void WriteSheetOfLightModel(const wchar_t* FileName) const;
#endif

  // Deserialize a sheet-of-light model.
  void DeserializeSheetOfLightModel(const HSerializedItem& SerializedItemHandle);

  // Serialize a sheet-of-light model.
  HSerializedItem SerializeSheetOfLightModel() const;

  // Calibrate a sheet-of-light setup with a 3D calibration object.
  double CalibrateSheetOfLight() const;

  // Get the result of a calibrated measurement performed with the  sheet-of-light technique as a 3D object model.
  HObjectModel3D GetSheetOfLightResultObjectModel3d() const;

  // Get the iconic results of a measurement performed with the sheet-of light technique.
  HImage GetSheetOfLightResult(const HTuple& ResultName) const;

  // Get the iconic results of a measurement performed with the sheet-of light technique.
  HImage GetSheetOfLightResult(const HString& ResultName) const;

  // Get the iconic results of a measurement performed with the sheet-of light technique.
  HImage GetSheetOfLightResult(const char* ResultName) const;

#ifdef _WIN32
  // Get the iconic results of a measurement performed with the sheet-of light technique.
  HImage GetSheetOfLightResult(const wchar_t* ResultName) const;
#endif

  // Apply the calibration transformations to the input disparity image.
  void ApplySheetOfLightCalibration(const HImage& Disparity) const;

  // Set sheet of light profiles by measured disparities.
  void SetProfileSheetOfLight(const HImage& ProfileDisparityImage, const HTuple& MovementPoses) const;

  // Process the profile image provided as input and store the resulting disparity to the sheet-of-light model.
  void MeasureProfileSheetOfLight(const HImage& ProfileImage, const HTuple& MovementPose) const;

  // Set selected parameters of the sheet-of-light model.
  void SetSheetOfLightParam(const HString& GenParamName, const HTuple& GenParamValue) const;

  // Set selected parameters of the sheet-of-light model.
  void SetSheetOfLightParam(const HString& GenParamName, const HString& GenParamValue) const;

  // Set selected parameters of the sheet-of-light model.
  void SetSheetOfLightParam(const char* GenParamName, const char* GenParamValue) const;

#ifdef _WIN32
  // Set selected parameters of the sheet-of-light model.
  void SetSheetOfLightParam(const wchar_t* GenParamName, const wchar_t* GenParamValue) const;
#endif

  // Get the value of a parameter, which has been set in a sheet-of-light model.
  HTuple GetSheetOfLightParam(const HString& GenParamName) const;

  // Get the value of a parameter, which has been set in a sheet-of-light model.
  HTuple GetSheetOfLightParam(const char* GenParamName) const;

#ifdef _WIN32
  // Get the value of a parameter, which has been set in a sheet-of-light model.
  HTuple GetSheetOfLightParam(const wchar_t* GenParamName) const;
#endif

  // For a given sheet-of-light model get the names of the generic iconic or control parameters that can be used in the different sheet-of-light operators.
  HTuple QuerySheetOfLightParams(const HString& QueryName) const;

  // For a given sheet-of-light model get the names of the generic iconic or control parameters that can be used in the different sheet-of-light operators.
  HTuple QuerySheetOfLightParams(const char* QueryName) const;

#ifdef _WIN32
  // For a given sheet-of-light model get the names of the generic iconic or control parameters that can be used in the different sheet-of-light operators.
  HTuple QuerySheetOfLightParams(const wchar_t* QueryName) const;
#endif

  // Reset a sheet-of-light model.
  void ResetSheetOfLightModel() const;

  // Delete a sheet-of-light model and free the allocated memory.
  void ClearSheetOfLightModel() const;

  // Create a model to perform 3D-measurements using the sheet-of-light technique.
  void CreateSheetOfLightModel(const HRegion& ProfileRegion, const HTuple& GenParamName, const HTuple& GenParamValue);

  // Create a model to perform 3D-measurements using the sheet-of-light technique.
  void CreateSheetOfLightModel(const HRegion& ProfileRegion, const HString& GenParamName, Hlong GenParamValue);

  // Create a model to perform 3D-measurements using the sheet-of-light technique.
  void CreateSheetOfLightModel(const HRegion& ProfileRegion, const char* GenParamName, Hlong GenParamValue);

#ifdef _WIN32
  // Create a model to perform 3D-measurements using the sheet-of-light technique.
  void CreateSheetOfLightModel(const HRegion& ProfileRegion, const wchar_t* GenParamName, Hlong GenParamValue);
#endif

};

// forward declarations and types for internal array implementation

template<class T> class HSmartPtr;
template<class T> class HHandleBaseArrayRef;

typedef HHandleBaseArrayRef<HSheetOfLightModel> HSheetOfLightModelArrayRef;
typedef HSmartPtr< HSheetOfLightModelArrayRef > HSheetOfLightModelArrayPtr;


// Represents multiple tool instances
class LIntExport HSheetOfLightModelArray : public HHandleBaseArray
{

public:

  // Create empty array
  HSheetOfLightModelArray();

  // Create array from native array of tool instances
  HSheetOfLightModelArray(HSheetOfLightModel* classes, Hlong length);

  // Copy constructor
  HSheetOfLightModelArray(const HSheetOfLightModelArray &tool_array);

  // Destructor
  virtual ~HSheetOfLightModelArray();

  // Assignment operator
  HSheetOfLightModelArray &operator=(const HSheetOfLightModelArray &tool_array);

  // Clears array and all tool instances
  virtual void Clear();

  // Get array of native tool instances
  const HSheetOfLightModel* Tools() const;

  // Get number of tools
  virtual Hlong Length() const;

  // Create tool array from tuple of handles
  virtual void SetFromTuple(const HTuple& handles);

  // Get tuple of handles for tool array
  virtual HTuple ConvertToTuple() const;

protected:

// Smart pointer to internal data container
   HSheetOfLightModelArrayPtr *mArrayPtr;
};

}

#endif
