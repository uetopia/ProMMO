// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef SIOJEDITORPLUGIN_SIOJ_BreakJson_generated_h
#error "SIOJ_BreakJson.generated.h already included, missing '#pragma once' in SIOJ_BreakJson.h"
#endif
#define SIOJEDITORPLUGIN_SIOJ_BreakJson_generated_h

#define ProMMO_Plugins_SocketIOClient_Source_SIOJEditorPlugin_Public_SIOJ_BreakJson_h_34_GENERATED_BODY \
	friend struct Z_Construct_UScriptStruct_FSIOJ_NamedType_Statics; \
	SIOJEDITORPLUGIN_API static class UScriptStruct* StaticStruct();


#define ProMMO_Plugins_SocketIOClient_Source_SIOJEditorPlugin_Public_SIOJ_BreakJson_h_49_RPC_WRAPPERS
#define ProMMO_Plugins_SocketIOClient_Source_SIOJEditorPlugin_Public_SIOJ_BreakJson_h_49_RPC_WRAPPERS_NO_PURE_DECLS
#define ProMMO_Plugins_SocketIOClient_Source_SIOJEditorPlugin_Public_SIOJ_BreakJson_h_49_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUSIOJ_BreakJson(); \
	friend struct Z_Construct_UClass_USIOJ_BreakJson_Statics; \
public: \
	DECLARE_CLASS(USIOJ_BreakJson, UK2Node, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/SIOJEditorPlugin"), NO_API) \
	DECLARE_SERIALIZER(USIOJ_BreakJson)


#define ProMMO_Plugins_SocketIOClient_Source_SIOJEditorPlugin_Public_SIOJ_BreakJson_h_49_INCLASS \
private: \
	static void StaticRegisterNativesUSIOJ_BreakJson(); \
	friend struct Z_Construct_UClass_USIOJ_BreakJson_Statics; \
public: \
	DECLARE_CLASS(USIOJ_BreakJson, UK2Node, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/SIOJEditorPlugin"), NO_API) \
	DECLARE_SERIALIZER(USIOJ_BreakJson)


#define ProMMO_Plugins_SocketIOClient_Source_SIOJEditorPlugin_Public_SIOJ_BreakJson_h_49_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API USIOJ_BreakJson(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(USIOJ_BreakJson) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, USIOJ_BreakJson); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(USIOJ_BreakJson); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API USIOJ_BreakJson(USIOJ_BreakJson&&); \
	NO_API USIOJ_BreakJson(const USIOJ_BreakJson&); \
public:


#define ProMMO_Plugins_SocketIOClient_Source_SIOJEditorPlugin_Public_SIOJ_BreakJson_h_49_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API USIOJ_BreakJson(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()) : Super(ObjectInitializer) { }; \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API USIOJ_BreakJson(USIOJ_BreakJson&&); \
	NO_API USIOJ_BreakJson(const USIOJ_BreakJson&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, USIOJ_BreakJson); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(USIOJ_BreakJson); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(USIOJ_BreakJson)


#define ProMMO_Plugins_SocketIOClient_Source_SIOJEditorPlugin_Public_SIOJ_BreakJson_h_49_PRIVATE_PROPERTY_OFFSET
#define ProMMO_Plugins_SocketIOClient_Source_SIOJEditorPlugin_Public_SIOJ_BreakJson_h_46_PROLOG
#define ProMMO_Plugins_SocketIOClient_Source_SIOJEditorPlugin_Public_SIOJ_BreakJson_h_49_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	ProMMO_Plugins_SocketIOClient_Source_SIOJEditorPlugin_Public_SIOJ_BreakJson_h_49_PRIVATE_PROPERTY_OFFSET \
	ProMMO_Plugins_SocketIOClient_Source_SIOJEditorPlugin_Public_SIOJ_BreakJson_h_49_RPC_WRAPPERS \
	ProMMO_Plugins_SocketIOClient_Source_SIOJEditorPlugin_Public_SIOJ_BreakJson_h_49_INCLASS \
	ProMMO_Plugins_SocketIOClient_Source_SIOJEditorPlugin_Public_SIOJ_BreakJson_h_49_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define ProMMO_Plugins_SocketIOClient_Source_SIOJEditorPlugin_Public_SIOJ_BreakJson_h_49_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	ProMMO_Plugins_SocketIOClient_Source_SIOJEditorPlugin_Public_SIOJ_BreakJson_h_49_PRIVATE_PROPERTY_OFFSET \
	ProMMO_Plugins_SocketIOClient_Source_SIOJEditorPlugin_Public_SIOJ_BreakJson_h_49_RPC_WRAPPERS_NO_PURE_DECLS \
	ProMMO_Plugins_SocketIOClient_Source_SIOJEditorPlugin_Public_SIOJ_BreakJson_h_49_INCLASS_NO_PURE_DECLS \
	ProMMO_Plugins_SocketIOClient_Source_SIOJEditorPlugin_Public_SIOJ_BreakJson_h_49_ENHANCED_CONSTRUCTORS \
static_assert(false, "Unknown access specifier for GENERATED_BODY() macro in class SIOJ_BreakJson."); \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID ProMMO_Plugins_SocketIOClient_Source_SIOJEditorPlugin_Public_SIOJ_BreakJson_h


#define FOREACH_ENUM_ESIOJ_JSONTYPE(op) \
	op(ESIOJ_JsonType::JSON_Bool) \
	op(ESIOJ_JsonType::JSON_Number) \
	op(ESIOJ_JsonType::JSON_String) \
	op(ESIOJ_JsonType::JSON_Object) 
PRAGMA_ENABLE_DEPRECATION_WARNINGS
