![gstreamer-plugin](docs/media/consume_stream.gif)

# Setup

Engine:
--------
- To use this plugin first `Clone` this repository in the `Plugins/Simbotic` directory of your  Engine:

```
git clone git@github.com:Simbotic/Streamer.git
```
- After clone the repository, recompile the engine using `make` .

Project:
--------

- To use the plugin on a project, update your `.uproject` adding the `UE4 Gstreamer Plugin` as an additional dependency on `.uproject`:

```
...,
"Modules": [
	{
		"Name": "GST_Test",
		"Type": "Runtime",
		"LoadingPhase": "Default",
		"AdditionalDependencies": [
			"Engine"
		]
	}
],
"Plugins": [
	{
		"Name": "GStreamer",
		"Enabled": true
	},
],
...

```
- Add, in the constructors of the classes of your project: `Source/PROJECT_NAME.Target.cs` and `Source/PROJECT_NAMEEditor.Target.cs` the following:
```
bUseUnityBuild = false;
bUsePCHFiles = false;
```

# Usage

On Blueprints the Blueprint hierarchy you need:

* `GstPipeline`.
* 2 `GstAppSrc` these components will be named as `GstAppSrcRGB` and `GstAppSrcDepth`.
* 2 `scene Capture Component 2D` these components will be named as `SceneCaptureRGB` and `SceneCaptureDepth`.



![gst-plugin-ue4 blueprints](docs/media/BP_Components.png)

# GstPipeline #
![gst-plugin-ue4 blueprints](docs/media/GstPipelineConfig.png)

*  `Pipeline Name` will be set as **robot** 
* **Pipeline config** will set as:

	> ``` appsrc name=sensor_rgb caps=video/x-raw,width=512,height=512,format=BGRx, framerate=10/1 ! videoconvert ! queue2 ! fpsdisplaysink ```
* **Pipeline Config File** will be empty.
* **Pipeline Use File** will be unchecked.
* **Pipeline Autostart** will be checked.


# GstAppSrc #

The components `GstAppSrc` are named as `GstAppSrcRGB` and `GstAppSrcDepth` these components are related with `Scene Capture Components` 


![gst-plugin-ue4 blueprints](docs/media/GstAppSrc.png)
![gst-plugin-ue4 blueprints](docs/media/GstAppSrcDepth.png)

*  **Pipeline Name** will be set as **robot**. 
* **App Src Name** it will be set as `sensor_rgb` and `sensor_depth` respectively.
* **App Src Enabled** will be set as **checked.** 
* **Src Video** it will be set as **SceneCaptureRGB** and **SceneCaptureDepth.**
* **Src Klv** Will Set as empty.


# Scene Capture Component 2D (General)

on Scene Capture component go to `Post Process Volume/Rendering/Features/Post process Materials/Array` 

![gst-plugin-ue4 blueprints](docs/media/SceneCapture.png)
![gst-plugin-ue4 blueprints](docs/media/SceneCaptureD.png)

## Materials
>![gst-plugin-ue4 blueprints](docs/media/M_SensorBGRA.png)
>![gst-plugin-ue4 blueprints](docs/media/M_SensorDepth.png)


 create a new array, on `choose` select  `asset reference`, then add the respectively Material `M_SensorBGRA` and `M_SensorDepth`.

 # Scene Capture Component 2D (RGB)

 Modify tab `Scene Capture`

 ![gst-plugin-ue4 blueprints](docs/media/SceneCaptureRGB.png)

 * On **Primitive Render Mode** select ` Render Scene Primitives` 
 * On **Capture Source** select ` Final Color (LDR) in RGB` 


# Scene Capture Component 2D (Depth)
 Modify tab `Scene Capture`

![gst-plugin-ue4 blueprints](docs/media/SceneCaptureDepth1.png)

 
 * On **Capture Source** select ` Final Color (LDR) in RGB` 