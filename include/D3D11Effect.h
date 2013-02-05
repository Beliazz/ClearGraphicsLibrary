#ifndef D3D11Effect_h__
#define D3D11Effect_h__

#include "cgl.h"

namespace cgl
{
	namespace core
	{

		//////////////////////////////////////////////////////////////////////////
		// typedefs
		class D3D11Effect;
		class D3D11EffectVariable;
		class D3D11EffectTechnique;
		class D3D11EffectPass;
		typedef CGL_API std::tr1::shared_ptr<D3D11Effect> PD3D11Effect;
		typedef CGL_API std::tr1::shared_ptr<D3D11EffectVariable> PD3D11EffectVariable;
		typedef CGL_API std::tr1::shared_ptr<D3D11EffectTechnique> PD3D11EffectTechnique;
		typedef CGL_API std::tr1::shared_ptr<D3D11EffectPass> PD3D11EffectPass;

		//////////////////////////////////////////////////////////////////////////
		// effect base class
		class CGL_API D3D11Effect : public CGLBase<ID3DX11Effect>
		{
		protected:
			void getDependencies(std::vector<PCGLObject>* pDependencies ) { }
			void onReset();

		public:
			D3D11Effect(std::string className);

			UINT Techniques();
			UINT ConstantBuffers();
		};

		//////////////////////////////////////////////////////////////////////////
		// effect from memory
		class CGL_API CD3D11EffectFromMemory : public D3D11Effect
		{
		private:
			std::tr1::shared_ptr<void> m_pBuffer;
			SIZE_T m_size;

		protected:
			CD3D11EffectFromMemory(void* pBuffer, SIZE_T size);
			HRESULT onRestore();

		public:
			static PD3D11Effect Create(void* pBuffer, SIZE_T size);
		};

		//////////////////////////////////////////////////////////////////////////
		// effect from file
		class CGL_API CD3D11EffectFromFile : public D3D11Effect
		{
		private:
			std::string m_fileName;

		protected:
			CD3D11EffectFromFile(std::string fileName);
			HRESULT onRestore();

		public:
			static PD3D11Effect Create(std::string fileName);
		};

		//////////////////////////////////////////////////////////////////////////
		// effect related object
		template <class T, class U>
		class CGL_API D3D11EffectObject : public CGLBase<U>
		{
		protected:
			T m_pDependence;
			void* m_pIdentifier;
	
			virtual void onReset()
			{	}
			inline void getDependencies(std::vector<cgl::core::PCGLObject>* pDependencies)
			{
				pDependencies->push_back(m_pDependence);
			}
			D3D11EffectObject(std::string className, T pDependence, void* pIdentifier, size_t size)
				: CGLBase(className), m_pDependence(pDependence)
			{
				m_pIdentifier = malloc(size);
				memcpy(m_pIdentifier, pIdentifier, size);
			}

		public:
			inline T GetDependency()		{ return m_pDependence; }
			inline void* GetIdentifier()	{ return m_pIdentifier; }

			virtual ~D3D11EffectObject()
			{
				free(m_pIdentifier);
			}
		};

		//////////////////////////////////////////////////////////////////////////
		// effect technique
		class CGL_API D3D11EffectTechnique : public D3D11EffectObject<PD3D11Effect, ID3DX11EffectTechnique>
		{
		protected:
			D3D11EffectTechnique(std::string className, PD3D11Effect pEffect, void* pIdentifier, size_t size);

		public:
			virtual ~D3D11EffectTechnique();

			UINT Passes();
		};

		//////////////////////////////////////////////////////////////////////////
		// effect technique from name
		class CGL_API CD3D11EffectTechniqueFromName : public D3D11EffectTechnique
		{
		protected:
			CD3D11EffectTechniqueFromName(PD3D11Effect pEffect, std::string name);
			HRESULT onRestore();

		public:
			static PD3D11EffectTechnique Create(PD3D11Effect pEffect, std::string name);
		};

		//////////////////////////////////////////////////////////////////////////
		// effect technique from index
		class CGL_API CD3D11EffectTechniqueFromIndex : public D3D11EffectTechnique
		{
		protected:
			CD3D11EffectTechniqueFromIndex(PD3D11Effect pEffect, UINT index);
			HRESULT onRestore();

		public:
			static PD3D11EffectTechnique Create(PD3D11Effect pEffect, UINT index);
		};

		//////////////////////////////////////////////////////////////////////////
		// effect pass
		class CGL_API D3D11EffectPass : public D3D11EffectObject<PD3D11EffectTechnique, ID3DX11EffectPass>
		{
		protected:
			D3D11EffectPass(std::string className, PD3D11EffectTechnique pTechnique, void* pIdentifier, size_t size);
	
		public:
			void Apply();
		};

		//////////////////////////////////////////////////////////////////////////
		// effect pass from name
		class CGL_API CD3D11EffectPassFromName : public D3D11EffectPass
		{
		protected:
			CD3D11EffectPassFromName(PD3D11EffectTechnique pTechnique, std::string name);
			HRESULT onRestore();

		public:
			static PD3D11EffectPass Create(PD3D11EffectTechnique pEffect, std::string name);
		};

		//////////////////////////////////////////////////////////////////////////
		// effect pass from index
		class CGL_API CD3D11EffectPassFromIndex : public D3D11EffectPass
		{
		protected:
			CD3D11EffectPassFromIndex(PD3D11EffectTechnique pTechnique, UINT index);
			HRESULT onRestore();

		public:
			static PD3D11EffectPass Create(PD3D11EffectTechnique pTechnique, UINT index);
		};

		//////////////////////////////////////////////////////////////////////////
		// effect variable
		class CGL_API D3D11EffectVariable : public D3D11EffectObject<PD3D11Effect, ID3DX11EffectVariable>
		{
		protected:
			D3D11EffectVariable(std::string className, PD3D11Effect pEffect, void* pIdentifier, size_t size);

		public:
			virtual ~D3D11EffectVariable();
			bool CheckType(D3D10_SHADER_VARIABLE_TYPE type);
		};

		//////////////////////////////////////////////////////////////////////////
		// effect variable from name
		class CGL_API CD3D11EffectVariableFromName : public D3D11EffectVariable
		{
		protected:
			CD3D11EffectVariableFromName(PD3D11Effect pEffect, std::string name);
			HRESULT onRestore();

		public:
			static PD3D11EffectVariable Create(PD3D11Effect pEffect, std::string name);
		};

		//////////////////////////////////////////////////////////////////////////
		// effect variable from semantic
		class CGL_API CD3D11EffectVariableFromSemantic : public D3D11EffectVariable
		{
		protected:
			CD3D11EffectVariableFromSemantic(PD3D11Effect pEffect, std::string semantic);
			HRESULT onRestore();

		public:
			static PD3D11EffectVariable Create(PD3D11Effect pEffect, std::string semantic);
		};

		//////////////////////////////////////////////////////////////////////////
		// effect variable form index
		class CGL_API CD3D11EffectVariableFromIndex : public D3D11EffectVariable
		{
		protected:
			CD3D11EffectVariableFromIndex(PD3D11Effect pEffect, UINT index);
			HRESULT onRestore();

		public:
			static PD3D11EffectVariable Create(PD3D11Effect pEffect, UINT index);
		};
	}
}
#endif // D3D11Effect_h__