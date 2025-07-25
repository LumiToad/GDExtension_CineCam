<div align="left">
  
  <h1>CineCam GDExtension for Godot 4.2.2</h1>

  <p>
    This GDExtension was developed as part of my final thesis at the <a href="https://www.school4games.net">School For Games</a> in Berlin! <br />
    A Camera addon for Godot 4.2.2.<br />
    Loosely based on the Unity Cinemachine.<br /><br />
    Makes blending and shaking cameras easy!<br />
    Aimed to reduce the amount of code lines in your project a lot.<br />
    A lot can be configured in the Inspector. <br /> <br />
  </p>
<img src="https://github.com/LumiToad/GDExtension_CineCam/assets/129980440/64f31142-a6fe-4a3f-904a-bee4cbefa347" width="200" height="200" /><br />
<details>
  <summary>Showcase GIFs</summary>
  <img src="https://github.com/LumiToad/GDExtension_CineCam/assets/129980440/3e3142ef-dd92-4de6-94e2-6ec2e43444b0" width="400" height="300" />
  <img src="https://github.com/LumiToad/GDExtension_CineCam/assets/129980440/3e5eddd0-e32d-4714-8111-67256ca49d4a" width="400" height="300" />
  <img src="https://github.com/LumiToad/GDExtension_CineCam/assets/129980440/2a728dc9-9588-42a5-bf3f-fbc30210304e" width="400" height="300" />
  <img src="https://github.com/LumiToad/GDExtension_CineCam/assets/129980440/0645f7f7-64ff-49d6-bcb4-da0a04c026c4" width="400" height="300" />
</details>

  <hr />
  <h2>Engines / Languages</h2>
  Godot 4.2.2, <a href="https://docs.godotengine.org/en/stable/tutorials/scripting/gdextension/what_is_gdextension.html">GDExtension API</a> -> C++, SCons -> Python

  <hr />
  <h2>Features in 2D and 3D</h2>
  <ul>
  <li>
    <a href="https://github.com/LumiToad/GDExtension_CineCam/wiki/Feature-%E2%80%90-Camera-blending-by-priority-%E2%80%90-CineCam-wiki">Camera blending</a><br />
    Includes infinite blending and one-time blending.<br />
    Both can be setup with different speed and interpolation settings.<br />
  </li>
    <br />
  <li>
    <a href="https://github.com/LumiToad/GDExtension_CineCam/wiki/Feature-%E2%80%90-Setup-virtual-camera-data-for-later-use">Dummy (Virtual) camera setups for future use, containing data of a future camera setup.</a>
  </li>
    <br />
  <li>
    <a href="https://github.com/LumiToad/GDExtension_CineCam/wiki/Feature-%E2%80%90-Camera-blending-by-priority-%E2%80%90-CineCam-wiki">Priority camera, setup in the virtual cameras.</a><br />
    Includes moving and blending.<br />
  </li>  
    <br />
  <li>
    <a href="https://github.com/LumiToad/GDExtension_CineCam/wiki/Feature-%E2%80%90-Following-a-target">Following a camera target.</a><br />
    Includes infinite blending and offset settings, with varying interpolation settings.
  </li>  
    <br />
  <li>
    Signals and callables<br />
    Trigger on certain moments like the completion of a blend.
  </li>  
    <br />
  <li>
    Blends can be paused and resumed.
  </li>
    <br />
  <li>
    Many setters / getters to retrieve information of the current status of the camera.
  </li>  
    <br />
  <li>
    Custom warnings and errors<br />
    Will safely ignore the error and provides information about the problem.
  </li>
    <br />
  <li>
    Utility features<br />
    For example: Moving to a given virtual camera setup or checking,<br />
    which is the next camera in the sequence. For a complete list, see class reference.<br />
  </li>
  </ul>

  2D only

   <ul>
  <li>
    <a href="https://github.com/LumiToad/GDExtension_CineCam/wiki/Feature-%E2%80%90-Easy-camera-shaking-%E2%80%90-CineCam-wiki">2D Camera shaking</a><br />
    Includes shaking using the camera frame <b>offset</b>, <b>zoom</b> and <b>rotation</b>.</br >
    All three can be setup with varying intensity, duration and interpolation settings.</br >
    Can be used while blending.
  </li>
     <br />
  <li>
    <a href="https://github.com/LumiToad/GDExtension_CineCam/wiki/Feature-%E2%80%90-Using-CamSequence-for-a-series-of-blends">Following a sequence of 2D blends</a><br />
    Includes editor integration, which shows the sequence as debug lines.
  </li>
    <br />
   </ul>

3D only

   <ul>
  <li>
    <a href="https://github.com/LumiToad/GDExtension_CineCam/wiki/Feature-%E2%80%90-Easy-camera-shaking-%E2%80%90-CineCam-wiki">3D Camera shaking</a><br />
    Includes shaking using the camera frame <b>offset</b>, <b>FOV</b> and <b>rotation</b>.</br >
    All three can be setup with varying intensity, duration and interpolation settings.</br >
    Can be used while blending.
  </li>
     <br />
  <li>
    <a href="https://github.com/LumiToad/GDExtension_CineCam/wiki/Feature-%E2%80%90-Using-CamSequence-for-a-series-of-blends">Following a sequence of 3D blends</a><br />
  </li>
     <br />
  <li>
    Rotation blending in 3D space   
  </li>
    <br />
  <li>
    <a href="https://github.com/LumiToad/GDExtension_CineCam/wiki/Feature-%E2%80%90-Following-a-target">Looking at a target</a><br />
    Includes infinite blending and offset settings, with varying interpolation settings.
  </li>
   </ul>
  
  <hr />
  <h2>Downloads and Website</h2>

  <h3>CineCam in the Godot Asset Library</h3>
  <a href="https://godotengine.org/asset-library/asset/3027">
    <img src="https://github.com/LumiToad/GDExtension_CineCam/raw/main/images/asset_library.png" width="230" height="45" alt="cinecam banner" />
  </a>

  <h3><a href="https://github.com/LumiToad/CineCam_Example">Example project</a></h3>
  This example project is a showcase of some features!
</div>
