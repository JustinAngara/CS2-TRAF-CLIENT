#include "ConvertToJson.h"
#include "../../libs/json.hpp"
#include <iostream>

using json = nlohmann::json;

/*

========================================
BLOCK #538
Type: NAMESPACE
Name: inv_image_light_sun_t
Items: 3
========================================
  Name: color | Offset: 0x0 | Type: Vector
  Name: angle | Offset: 0xC | Type: QAngle
  Name: brightness | Offset: 0x18 | Type: float32


========================================
BLOCK #539
Type: NAMESPACE
Name: CompositeMaterialMatchFilter_t
Items: 4
========================================
  Name: m_nCompositeMaterialMatchFilterType | Offset: 0x0 | Type: CompositeMaterialMatchFilterType_t
  Name: m_strMatchFilter | Offset: 0x8 | Type: CUtlString
  Name: m_strMatchValue | Offset: 0x10 | Type: CUtlString
  Name: m_bPassWhenTrue | Offset: 0x18 | Type: bool


========================================
BLOCK #540
Type: NAMESPACE
Name: CompositeMaterial_t
Items: 4
========================================
  Name: m_TargetKVs | Offset: 0x8 | Type: KeyValues3
  Name: m_PreGenerationKVs | Offset: 0x18 | Type: KeyValues3
  Name: m_FinalKVs | Offset: 0x58 | Type: KeyValues3
  Name: m_vecGeneratedTextures | Offset: 0x80 | Type: CUtlVector<GeneratedTextureHandle_t>

*/

/*

obj.data[0] -> first instance of object

{
	"data":[
		{
			"Key" : 530,
			"Type" : "NAMESPACE",
			"Name" : "CompositeMaterial_t",
			"Items" : 4,
			"List": [
				["m_targetKVS", 0x8, "KeyValues3"],
				["m_PreGenerationKVs", 0x17, "KeyValues3"],
				...
			]
		},

		{
			Key....
		}
	]
}

*/

void ConvertToJson::populateContent()
{
	json j;
	j["data"] = json::array();

	// Add objects
	json obj1;
	obj1["Key"]	  = 530;
	obj1["Type"]  = "NAMESPACE";
	obj1["Name"]  = "CompositeMaterial_t";
	obj1["Items"] = 4;
	obj1["List"]  = {
		 { "m_targetKVS", 0x8, "KeyValues3" },
		 { "m_PreGenerationKVs", 0x17, "KeyValues3" }
	};

	j["data"].push_back(obj1);

	m_content = j.dump(2); 
}

void ConvertToJson::printJson()
{
	std::cout << m_content;
}


