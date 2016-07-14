#ifndef ANGELSCRIPT_CASMODULE_H
#define ANGELSCRIPT_CASMODULE_H

#include <cstring>

#include "util/CASBaseClass.h"

#include "CASModuleDescriptor.h"

class asIScriptModule;

/**
*	The user data ID for the CASModule instance in asIScriptModule.
*/
#define CASMODULE_USER_DATA_ID 10001

/**
*	Angelscript module. Wraps asIScriptModule and provides the descriptor.
*	Is reference counted, unlike the script module.
*/
class CASModule final : public CASAtomicRefCountedBaseClass
{
public:
	/**
	*	Constructor.
	*	@param pModule Script module.
	*	@param descriptor Descriptor for this module.
	*/
	CASModule( asIScriptModule* pModule, const CASModuleDescriptor& descriptor );

	/**
	*	Destructor.
	*/
	~CASModule();

	void Release() const;

	/**
	*	Discards the module. It can no longer be used after this.
	*/
	void Discard();

	/**
	*	@return The script module.
	*/
	asIScriptModule* GetModule() { return m_pModule; }

	/**
	*	@return The module name.
	*/
	const char* GetModuleName() const;

	/**
	*	@return The descriptor.
	*/
	const CASModuleDescriptor& GetDescriptor() const { return *m_pDescriptor; }

private:
	asIScriptModule* m_pModule;

	const CASModuleDescriptor* m_pDescriptor;

private:
	CASModule( const CASModule& ) = delete;
	CASModule& operator=( const CASModule& ) = delete;
};

/**
*	Gets a module from a script module.
*	@param pModule Script module to retrieve the module from.
*	@return The module, or null if it couldn't be retrieved.
*/
CASModule* GetModuleFromScriptModule( const asIScriptModule* pModule );

/**
*	Gets a module from a script function.
*	@param pModule Script function to retrieve the module from.
*	@return The module, or null if it couldn't be retrieved.
*/
CASModule* GetModuleFromScriptFunction( const asIScriptFunction* pFunction );

/**
*	Less function for modules.
*	@param pLHS Left hand module.
*	@param pRHS Right hand module.
*	@return true if the left hand module is smaller than the right hand module.
*/
inline bool ModuleLess( const CASModule* pLHS, const CASModule* pRHS )
{
	if(  pLHS->GetDescriptor() < pRHS->GetDescriptor() )
		return true;

	if( pRHS->GetDescriptor() < pLHS->GetDescriptor() )
		return false;

	//Use the memory address for sorting. They need only be sorted in a unique order.
	return pLHS < pRHS;
}

/**
*	Functor that overloads operator() to return true if a module equals a given name.
*/
struct ModuleEqualByName final
{
	const char* const pszModuleName;

	ModuleEqualByName( const char* const pszModuleName )
		: pszModuleName( pszModuleName )
	{
	}

	bool operator()( const CASModule* pModule ) const
	{
		return strcmp( pModule->GetModuleName(), pszModuleName ) == 0;
	}
};

#endif //ANGELSCRIPT_CASMODULE_H