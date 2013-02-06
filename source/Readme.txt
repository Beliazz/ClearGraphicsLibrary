<h1>CLEAR GRAPHICS LIBRARY</h1>					
<h2>D3D11 for beginner</h2>	
					
<h3>How to build</h3>

<ol>
	<li>checkout from github</li>
	<li>fire up visual studio
	<li>add include and library paths for both directX and
   	    the effect library (DirectXDir\samples\c++\effects11)</li>
	<li>build</li>
</ol>

<h3>How to use</h3>

Add include and library paths to your project and copy the built dll
to your project dir.

```c++
#include <cgl.h>
#pragma comment (lib, "ClearGraphicsLibrary.lib")

using namespace cgl;
```

first create the cgl manager

```
CGLManager cglMgr;
```

then if you don't have your own window class you can use the cgl window class

```
PCGLWindow pWindow = PCGLWindow(new cgl::CGLWindow(L"read me", 640, 480, Framework::WindowProc));
```

now initialize it with a simple call to restore. All cgl core objects are restored this way.

```
if (!pWindow->restore())
	return 1;
```
the next step is to create the dxgi factory to get the available adapters.
All core components are created using a static function called Create(). 

```
PDXGIFactory pFactory = CDXGIFactory::Create();
```

now retrieve the first adapter (for simplicity)

```
PDXGIADApter pAdapter = CDXGIAdapter::Create(pFactory, 0);
```

and for this adapter the first output

```
PDXGIOutput pOutput = CDXGIOutput::Create(pAdapter, 0);
if (!pOutput->restore())
	return 1;
```

maybe you have noticed that neither the DXGI Factory the DXGIAdapter have been restored. 
This will be done automatically as pOutput depends on them.


note: Every core object like CDXGIOuput or CD3D11Device wraps an COM Interface which is accessible
through get() and ptr().
This applies also to some helper classes like CGLWindow which wraps
a window handle (HWND) which will be used for the device creation.


now we can obtain the list of available formats for this adapter and 
the specified output.

```
vector<DXGI_MODE_DESC> modes = pOutput->GetFormats();
DXGI_MODE_DESC selectedMode = modes[modes.size() - 1];
```

for this example the last mode will be used

the next step will be creating the d3d device but first
we have to fill the device desc.

note: the member pAdapter has to be NULL on pc's with switchable gpu's

```
CGL_D3D11_DEVICE_DESC deviceDesc;
ZeroMemory(&deviceDesc, sizeof(deviceDesc));

deviceDesc.pAdapter = NULL;
deviceDesc.flags = D3D11_CREATE_DEVICE_DEBUG;
deviceDesc.driverType = D3D_DRIVER_TYPE_HARDWARE;
deviceDesc.featureLevel = D3D_FEATURE_LEVEL_10_1;
deviceDesc.swapDesc.OutputWindow = pWindow->get();
deviceDesc.swapDesc.SampleDesc.Count = 1;
deviceDesc.swapDesc.SampleDesc.Quality = 0;
deviceDesc.swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
deviceDesc.swapDesc.Windowed = TRUE;
deviceDesc.swapDesc.BufferCount = 1;
deviceDesc.swapDesc.BufferDesc = selectedMode;
deviceDesc.swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
```
now we are ready to create the device

note: device doesn't need to be restored as this happens automatically

```
PD3D11Device pDevice = CD3D11Device::Create(deviceDesc);
if (!pDevice)
	return 1;
```

this concludes the first preview. I will soon upload a sample project where
the most important d3d objects are used. if you are interested in further information
about this library just contact me. 

Remember: the most objects are all created the same way

```
P(ObjectName) pObject = C(ObjectName)::Create(param1, param2, ...);
if (!pObject->restore())
	return 1;
```

More tutorials on demand :)





