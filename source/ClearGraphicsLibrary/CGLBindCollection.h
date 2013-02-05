#ifndef CGLBindCollection_h__
#define CGLBindCollection_h__

#include "cgl.h"

namespace cgl
{
	namespace core
	{
		// typedef
		class CGLShaderResourceViewCollection;
		class CGLRenderTargetViewCollection;
		typedef CGL_API std::tr1::shared_ptr<CGLShaderResourceViewCollection> PCGLShaderResourceViewCollection; 
		typedef CGL_API std::tr1::shared_ptr<CGLRenderTargetViewCollection> PCGLRenderTargetViewCollection; 

		//////////////////////////////////////////////////////////////////////////
		// BindCollection
		template <class T>
		class CGL_API CGLBindCollection : public cgl::core::CGLBase<T*>
		{
		private:
			std::vector<std::tr1::shared_ptr<cgl::core::CGLBase<T>>> m_collection;

		protected:
			virtual HRESULT onRestore();
			virtual void onReset();

			virtual void OnAdd() { };
			virtual void OnRemove() { };
		public:
			CGLBindCollection(std::string className) : CGLBase(className)
			{
				//////////////////////////////////////////////////////////////////////////
				// template instantiation
				// 
				// this sick shit must be done otherwise
				// you get nasty linker errors
				// 
				// because of this explicit call
				// the compiler creates the template member functions
				// for the specific type
				Add(0, 0);
				Remove(0);
			}
			virtual ~CGLBindCollection() { onReset(); }
			virtual bool Update();

			inline void Add(std::tr1::shared_ptr<cgl::core::CGLBase<T>> object, UINT slot)
			{
				// resize vector
				while(slot >= m_collection.size()) {
					m_collection.push_back(NULL);
				}

				// set
				m_collection[slot] = object;

				OnAdd();
			}
			inline void Remove(UINT slot)
			{
				if (slot > 0 && slot < m_collection.size())
				{
					m_collection[slot] = NULL;
				}

				OnRemove();
			}
			inline std::vector<std::tr1::shared_ptr<cgl::core::CGLBase<T>>>& GetCollection() {return m_collection;}
			inline size_t GetCollectionSize() {return m_collection.size();}
		};
	}
}

#endif // CGLBindCollection_h__