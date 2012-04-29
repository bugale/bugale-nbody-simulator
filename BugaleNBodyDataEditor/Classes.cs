/*
    This is the Bugale N-Body Data Editor. A data editor for Bugale N-Body Simulator.
    Copyright (C) 2012  bugale
	
	You may contact me by email: bugalit@gmail.com

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 3 as published by
    the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
using System.ComponentModel;
using System.Drawing;
using System;
using System.Collections.Generic;

namespace BugaleNBodyDataEditor
{
    public enum AlgorithmType { ModifiedEuler = 0, Hermite = 1, Euler = 2, Leapfrog = 3 }
    public class MainSettings
    {
        private uint width;
        private uint height;
        private bool two_dimensional_calculation;
        private bool two_dimensional_graphic;
        private bool two_dimensional_binary;
        private bool fullscreen;
        private bool clear_screen;
        private bool show_trails;
        private bool min_text;
        private bool crosshair;
        private bool wireframe;
        private bool paused;
        private bool log;
        private bool opencl;
        private AlgorithmType algorithm;
        private double g;
        private double dt;
        private float graphic_max_rate;
        private float binary_max_rate;
        private long max_calculations;
        private uint max_trails;
        private uint stick_to_body;
        private uint sphere_slices;
        private uint sphere_stacks;
        private float field_of_view;
        private double near_plane_distance;
        private double far_plane_distance;
        private double camera_positionX;
        private double camera_positionY;
        private double camera_positionZ;
        private double camera_targetX;
        private double camera_targetY;
        private double camera_targetZ;
        private float camera_upX;
        private float camera_upY;
        private float camera_upZ;
        private float keyboard_move_speed0;
        private float keyboard_move_speed1;
        private float keyboard_zoom_speed0;
        private float keyboard_zoom_speed1;
        private uint cl_num_of_threads;
        private uint cl_threads_in_workgroup;
        private uint cl_calcs_in_run;

        public MainSettings()
        {
            this.width = 0;
            this.height = 0;
            this.two_dimensional_calculation = false;
            this.two_dimensional_graphic = false;
            this.two_dimensional_binary = false;
            this.fullscreen = false;
            this.clear_screen = true;
            this.show_trails = false;
            this.min_text = false;
            this.crosshair = true;
            this.wireframe = false;
            this.paused = false;
            this.log = false;
            this.opencl = false;
            this.algorithm = AlgorithmType.Hermite;
            this.g = 0;
            this.dt = 0;
            this.graphic_max_rate = 0;
            this.binary_max_rate = 0;
            this.max_calculations = 0;
            this.sphere_slices = 0;
            this.sphere_stacks = 0;
            this.field_of_view = 0;
            this.near_plane_distance = 0;
            this.far_plane_distance = 0;
            this.camera_positionX = 0;
            this.camera_positionY = 0;
            this.camera_positionZ = 0;
            this.camera_targetX = 0;
            this.camera_targetY = 0;
            this.camera_targetZ = 0;
            this.camera_upX = 0;
            this.camera_upY = 0;
            this.camera_upZ = 0;
            this.keyboard_move_speed0 = 0;
            this.keyboard_move_speed1 = 0;
            this.keyboard_zoom_speed0 = 0;
            this.keyboard_zoom_speed1 = 0;
            this.cl_num_of_threads = 2048;
            this.cl_threads_in_workgroup = 512;
        }

        [DescriptionAttribute("The width in pixels of the screen.")]
        public uint Width
        {
            get { return this.width; }
            set { this.width = value; }
        }

        [DescriptionAttribute("The height in pixels of the screen.")]
        public uint Height
        {
            get { return this.height; }
            set { this.height = value; }
        }

        [DescriptionAttribute("Determines whether the calculation will be done only for the x and y axis, or for x, y and z.")]
        public bool TwoDimensionalCalculation
        {
            get { return this.two_dimensional_calculation; }
            set { this.two_dimensional_calculation = value; }
        }

        [DescriptionAttribute("Determines whether the graphical output will be given only for the x and y axis, or for x, y and z.")]
        public bool TwoDimensionalGraphic
        {
            get { return this.two_dimensional_graphic; }
            set { this.two_dimensional_graphic = value; }
        }

        [DescriptionAttribute("Determines whether the binary output will be given only for the x and y axis, or for x, y and z.")]
        public bool TwoDimensionalBinary
        {
            get { return this.two_dimensional_binary; }
            set { this.two_dimensional_binary = value; }
        }

        [DescriptionAttribute("Determines whether the graphic output will be presented on full screen or not.")]
        public bool Fullscreen
        {
            get { return this.fullscreen; }
            set { this.fullscreen = value; }
        }

        [DescriptionAttribute("Determines whether the screen will be cleared. This way, some sort of \"trails\" effect will be achieved.")]
        public bool ClearScreen
        {
            get { return this.clear_screen; }
            set { this.clear_screen = value; }
        }

        [DescriptionAttribute("Determines whether the trails will be shown.")]
        public bool ShowTrails
        {
            get { return this.show_trails; }
            set { this.show_trails = value; }
        }

        [DescriptionAttribute("Determines whether minimal text mode will be used.")]
        public bool MinimalText
        {
            get { return this.min_text; }
            set { this.min_text = value; }
        }

        [DescriptionAttribute("Determines whether the crosshair will be shown.")]
        public bool Crosshair
        {
            get { return this.crosshair; }
            set { this.crosshair = value; }
        }

        [DescriptionAttribute("Determines whether the bodies and trails in the graphical output will be drawn as wireframes. Wireframs (as opposed to solid) drawing is much more faster but looks less realistic.")]
        public bool Wireframe
        {
            get { return this.wireframe; }
            set { this.wireframe = value; }
        }

        [DescriptionAttribute("Determines whether the simulation will be started in Pause mode..")]
        public bool Paused
        {
            get { return this.paused; }
            set { this.paused = value; }
        }

        [DescriptionAttribute("Determines whether the logging will be enabled or not.")]
        public bool Log
        {
            get { return this.log; }
            set { this.log = value; }
        }

        [DescriptionAttribute("Determines whether the OpenCL (GPU calculation) will be used when possible.")]
        public bool OpenCL
        {
            get { return this.opencl; }
            set { this.opencl = value; }
        }

        [DescriptionAttribute("Determines which integration algorithm will be used.")]
        public AlgorithmType Algorithm
        {
            get { return this.algorithm; }
            set { this.algorithm = value; }
        }

        [DescriptionAttribute("The gravitational constant in (m^3 kg^-1 s^-2).")]
        public double G
        {
            get { return this.g; }
            set { this.g = value; }
        }

        [DescriptionAttribute("The amount of time in simulated seconds for which the engine will not change the acceleration.")]
        public double DT
        {
            get { return this.dt; }
            set { this.dt = value; }
        }

        [DescriptionAttribute("The maximal rate in s^-1 of the graphical output update. Write 0 for disabling graphical output, Write a negative number for V-Sync.")]
        public float GraphicMaxRate
        {
            get { return this.graphic_max_rate; }
            set { this.graphic_max_rate = value; }
        }

        [DescriptionAttribute("The maximal rate in s^-1 of the binary output update. Write 0 for disabling textual output, write a negative number for updating as soon as possible.")]
        public float BinaryMaxRate
        {
            get { return this.binary_max_rate; }
            set { this.binary_max_rate = value; }
        }

        [DescriptionAttribute("The maximal amount of calculations to perform. Write 0 for disabling calculation. Write a negative number for unlimited calculation.")]
        public long MaxCalculations
        {
            get { return this.max_calculations; }
            set { this.max_calculations = value; }
        }

        [DescriptionAttribute("The maximal amount of trails saved for each body.")]
        public uint MaxTrails
        {
            get { return this.max_trails; }
            set { this.max_trails = value; }
        }

        [DescriptionAttribute("A body index to use continuously as the target in 3D graphical output.")]
        public uint StickToBody
        {
            get { return this.stick_to_body; }
            set { this.stick_to_body = value; }
        }

        [DescriptionAttribute("The number of slices in the rendered spheres. More slices = better looking spheres.")]
        public uint SphereSlices
        {
            get { return this.sphere_slices; }
            set { this.sphere_slices = value; }
        }

        [DescriptionAttribute("The number of stacks in the rendered spheres. More stacks = better looking spheres.")]
        public uint SphereStacks
        {
            get { return this.sphere_stacks; }
            set { this.sphere_stacks = value; }
        }

        [DescriptionAttribute("The horizontal field of view in angles.")]
        public float FieldOfView
        {
            get { return this.field_of_view; }
            set { this.field_of_view = value; }
        }

        [DescriptionAttribute("The distance between the camera and the nearest point you can see.")]
        public double NearPlaneDistance
        {
            get { return this.near_plane_distance; }
            set { this.near_plane_distance = value; }
        }

        [DescriptionAttribute("The distance between the camera and the farest point you can see.")]
        public double FarPlaneDistance
        {
            get { return this.far_plane_distance; }
            set { this.far_plane_distance = value; }
        }

        [DescriptionAttribute("The X value of the position of the camera.")]
        public double CameraPositionX
        {
            get { return this.camera_positionX; }
            set { this.camera_positionX = value; }
        }

        [DescriptionAttribute("The Y value of the position of the camera.")]
        public double CameraPositionY
        {
            get { return this.camera_positionY; }
            set { this.camera_positionY = value; }
        }

        [DescriptionAttribute("The Z value of the position of the camera.")]
        public double CameraPositionZ
        {
            get { return this.camera_positionZ; }
            set { this.camera_positionZ = value; }
        }

        [DescriptionAttribute("The X value of the target vector of the camera. Used only in 3D graphical output.")]
        public double CameraTargetX
        {
            get { return this.camera_targetX; }
            set { this.camera_targetX = value; }
        }

        [DescriptionAttribute("The Y value of the target vector of the camera. Used only in 3D graphical output.")]
        public double CameraTargetY
        {
            get { return this.camera_targetY; }
            set { this.camera_targetY = value; }
        }

        [DescriptionAttribute("The Z value of the target vector of the camera. Used only in 3D graphical output.")]
        public double CameraTargetZ
        {
            get { return this.camera_targetZ; }
            set { this.camera_targetZ = value; }
        }

        [DescriptionAttribute("The X value of the up vector of the camera. Used only in 3D graphical output.")]
        public float CameraUpX
        {
            get { return this.camera_upX; }
            set { this.camera_upX = value; }
        }

        [DescriptionAttribute("The Y value of the up vector of the camera. Used only in 3D graphical output.")]
        public float CameraUpY
        {
            get { return this.camera_upY; }
            set { this.camera_upY = value; }
        }

        [DescriptionAttribute("The Z value of the up vector of the camera. Used only in 3D graphical output.")]
        public float CameraUpZ
        {
            get { return this.camera_upZ; }
            set { this.camera_upZ = value; }
        }

        [DescriptionAttribute("The initial movement speed of the camera(2D)/target(3D) when using the keyboard, in screens per second.")]
        public float KeyboardMoveSpeed0
        {
            get { return this.keyboard_move_speed0; }
            set { this.keyboard_move_speed0 = value; }
        }

        [DescriptionAttribute("The movement speed of the camera(2D)/target(3D) after one second when using the keyboard, in screens per second.")]
        public float KeyboardMoveSpeed1
        {
            get { return this.keyboard_move_speed1; }
            set { this.keyboard_move_speed1 = value; }
        }

        [DescriptionAttribute("The initial zoom speed of the camera.")]
        public float KeyboardZoomSpeed0
        {
            get { return this.keyboard_zoom_speed0; }
            set { this.keyboard_zoom_speed0 = value; }
        }

        [DescriptionAttribute("The zoom speed of the camera after one second.")]
        public float KeyboardZoomSpeed1
        {
            get { return this.keyboard_zoom_speed1; }
            set { this.keyboard_zoom_speed1 = value; }
        }

        [DescriptionAttribute("The number of threads that will be used in OpenCL.")]
        public uint OpenCLNumberOfThreads
        {
            get { return this.cl_num_of_threads; }
            set { this.cl_num_of_threads = value; }
        }

        [DescriptionAttribute("The number of threads in each workgroup in OpenCL.")]
        public uint OpenCLNumberOfThreadsInWorkgroup
        {
            get { return this.cl_threads_in_workgroup; }
            set { this.cl_threads_in_workgroup = value; }
        }

        [DescriptionAttribute("The number of calculations in each run in OpenCL. A large number of calculations will make it more efficient, but the program will also hang for more time.")]
        public uint OpenCLNumberOfCalculationsInEachRun
        {
            get { return this.cl_calcs_in_run; }
            set { this.cl_calcs_in_run = value; }
        }

        public void FromBytes0(byte[] data, double g)
        {
            this.g = g;
            byte bools = data[0];
            int cur = 0;
            this.two_dimensional_calculation = ((bools >> cur++) & 1) == 1 ? true : false;
            this.two_dimensional_graphic = ((bools >> cur++) & 1) == 1 ? true : false;
            this.two_dimensional_binary = ((bools >> cur++) & 1) == 1 ? true : false;
            this.fullscreen = ((bools >> cur++) & 1) == 1 ? true : false;
            this.log = ((bools >> cur++) & 1) == 1 ? true : false;

            cur = 1;
            this.width = BitConverter.ToUInt32(data, cur); cur += sizeof(uint);
            this.height = BitConverter.ToUInt32(data, cur); cur += sizeof(uint);
            this.algorithm = (AlgorithmType)data[cur]; cur += sizeof(byte);
            this.dt = BitConverter.ToDouble(data, cur); cur += sizeof(double);
            this.graphic_max_rate = (float)BitConverter.ToDouble(data, cur); cur += sizeof(double);
            this.binary_max_rate = (float)BitConverter.ToDouble(data, cur); cur += sizeof(double);
            this.max_calculations = BitConverter.ToInt64(data, cur); cur += sizeof(long);
            this.max_trails = BitConverter.ToUInt32(data, cur); cur += sizeof(uint);
            this.sphere_slices = BitConverter.ToUInt32(data, cur); cur += sizeof(uint);
            this.sphere_stacks = BitConverter.ToUInt32(data, cur); cur += sizeof(uint);
            this.field_of_view = (float)BitConverter.ToDouble(data, cur); cur += sizeof(double);
            this.near_plane_distance = BitConverter.ToDouble(data, cur); cur += sizeof(double);
            this.far_plane_distance = BitConverter.ToDouble(data, cur); cur += sizeof(double);
            this.camera_positionX = BitConverter.ToDouble(data, cur); cur += sizeof(double);
            this.camera_positionY = BitConverter.ToDouble(data, cur); cur += sizeof(double);
            this.camera_positionZ = BitConverter.ToDouble(data, cur); cur += sizeof(double);
            this.camera_targetX = BitConverter.ToDouble(data, cur); cur += sizeof(double);
            this.camera_targetY = BitConverter.ToDouble(data, cur); cur += sizeof(double);
            this.camera_targetZ = BitConverter.ToDouble(data, cur); cur += sizeof(double);
            this.camera_upX = (float)BitConverter.ToDouble(data, cur); cur += sizeof(double);
            this.camera_upY = (float)BitConverter.ToDouble(data, cur); cur += sizeof(double);
            this.camera_upZ = (float)BitConverter.ToDouble(data, cur); cur += sizeof(double);
            this.keyboard_move_speed0 = (float)BitConverter.ToDouble(data, cur); cur += sizeof(double);
            this.keyboard_move_speed1 = (float)BitConverter.ToDouble(data, cur); cur += sizeof(double);
            this.keyboard_zoom_speed0 = (float)BitConverter.ToDouble(data, cur); cur += sizeof(double);
            this.keyboard_zoom_speed1 = (float)BitConverter.ToDouble(data, cur); cur += sizeof(double);
        }

        public void FromBytes1(byte[] data, double g)
        {
            this.g = g;
            ushort bools = (ushort)(((ushort)data[13] << 8) + (ushort)data[12]);
            int cur = 0;
            this.two_dimensional_calculation = ((bools >> cur++) & 1) == 1 ? true : false;
            this.two_dimensional_graphic = ((bools >> cur++) & 1) == 1 ? true : false;
            this.two_dimensional_binary = ((bools >> cur++) & 1) == 1 ? true : false;
            this.fullscreen = ((bools >> cur++) & 1) == 1 ? true : false;
            this.clear_screen = ((bools >> cur++) & 1) == 1 ? true : false;
            this.show_trails = ((bools >> cur++) & 1) == 1 ? true : false;
            this.min_text = ((bools >> cur++) & 1) == 1 ? true : false;
            this.crosshair = ((bools >> cur++) & 1) == 1 ? true : false;
            this.paused = ((bools >> cur++) & 1) == 1 ? true : false;
            this.log = ((bools >> cur++) & 1) == 1 ? true : false;

            cur = 14;
            this.width = BitConverter.ToUInt32(data, cur); cur += sizeof(uint);
            this.height = BitConverter.ToUInt32(data, cur); cur += sizeof(uint);
            this.algorithm = (AlgorithmType)data[cur]; cur += sizeof(byte);
            this.dt = BitConverter.ToDouble(data, cur); cur += sizeof(double);
            this.graphic_max_rate = (float)BitConverter.ToDouble(data, cur); cur += sizeof(double);
            this.binary_max_rate = (float)BitConverter.ToDouble(data, cur); cur += sizeof(double);
            this.max_calculations = BitConverter.ToInt64(data, cur); cur += sizeof(long);
            this.max_trails = BitConverter.ToUInt32(data, cur); cur += sizeof(int);
            this.stick_to_body = BitConverter.ToUInt32(data, cur); cur += sizeof(uint);
            this.sphere_slices = BitConverter.ToUInt32(data, cur); cur += sizeof(uint);
            this.sphere_stacks = BitConverter.ToUInt32(data, cur); cur += sizeof(uint);
            this.field_of_view = (float)BitConverter.ToDouble(data, cur); cur += sizeof(double);
            this.near_plane_distance = BitConverter.ToDouble(data, cur); cur += sizeof(double);
            this.far_plane_distance = BitConverter.ToDouble(data, cur); cur += sizeof(double);
            this.camera_positionX = BitConverter.ToDouble(data, cur); cur += sizeof(double);
            this.camera_positionY = BitConverter.ToDouble(data, cur); cur += sizeof(double);
            this.camera_positionZ = BitConverter.ToDouble(data, cur); cur += sizeof(double);
            this.camera_targetX = BitConverter.ToDouble(data, cur); cur += sizeof(double);
            this.camera_targetY = BitConverter.ToDouble(data, cur); cur += sizeof(double);
            this.camera_targetZ = BitConverter.ToDouble(data, cur); cur += sizeof(double);
            this.camera_upX = (float)BitConverter.ToDouble(data, cur); cur += sizeof(double);
            this.camera_upY = (float)BitConverter.ToDouble(data, cur); cur += sizeof(double);
            this.camera_upZ = (float)BitConverter.ToDouble(data, cur); cur += sizeof(double);
            this.keyboard_move_speed0 = (float)BitConverter.ToDouble(data, cur); cur += sizeof(double);
            this.keyboard_move_speed1 = (float)BitConverter.ToDouble(data, cur); cur += sizeof(double);
            this.keyboard_zoom_speed0 = (float)BitConverter.ToDouble(data, cur); cur += sizeof(double);
            this.keyboard_zoom_speed1 = (float)BitConverter.ToDouble(data, cur); cur += sizeof(double);
        }

        public void FromBytes2(byte[] data, double g)
        {
            this.g = g;
            ushort bools = (ushort)(((ushort)data[13] << 8) + (ushort)data[12]);
            int cur = 0;
            this.two_dimensional_calculation = ((bools >> cur++) & 1) == 1 ? true : false;
            this.two_dimensional_graphic = ((bools >> cur++) & 1) == 1 ? true : false;
            this.two_dimensional_binary = ((bools >> cur++) & 1) == 1 ? true : false;
            this.fullscreen = ((bools >> cur++) & 1) == 1 ? true : false;
            this.clear_screen = ((bools >> cur++) & 1) == 1 ? true : false;
            this.show_trails = ((bools >> cur++) & 1) == 1 ? true : false;
            this.min_text = ((bools >> cur++) & 1) == 1 ? true : false;
            this.crosshair = ((bools >> cur++) & 1) == 1 ? true : false;
            this.wireframe = ((bools >> cur++) & 1) == 1 ? true : false;
            this.paused = ((bools >> cur++) & 1) == 1 ? true : false;
            this.log = ((bools >> cur++) & 1) == 1 ? true : false;

            cur = 14;
            this.width = BitConverter.ToUInt32(data, cur); cur += sizeof(uint);
            this.height = BitConverter.ToUInt32(data, cur); cur += sizeof(uint);
            this.algorithm = (AlgorithmType)data[cur]; cur += sizeof(byte);
            this.dt = BitConverter.ToDouble(data, cur); cur += sizeof(double);
            this.graphic_max_rate = BitConverter.ToSingle(data, cur); cur += sizeof(float);
            this.binary_max_rate = BitConverter.ToSingle(data, cur); cur += sizeof(float);
            this.max_calculations = BitConverter.ToInt64(data, cur); cur += sizeof(long);
            this.max_trails = BitConverter.ToUInt32(data, cur); cur += sizeof(int);
            this.stick_to_body = BitConverter.ToUInt32(data, cur); cur += sizeof(uint);
            this.sphere_slices = BitConverter.ToUInt32(data, cur); cur += sizeof(uint);
            this.sphere_stacks = BitConverter.ToUInt32(data, cur); cur += sizeof(uint);
            this.field_of_view = BitConverter.ToSingle(data, cur); cur += sizeof(float);
            this.near_plane_distance = BitConverter.ToDouble(data, cur); cur += sizeof(double);
            this.far_plane_distance = BitConverter.ToDouble(data, cur); cur += sizeof(double);
            this.camera_positionX = BitConverter.ToDouble(data, cur); cur += sizeof(double);
            this.camera_positionY = BitConverter.ToDouble(data, cur); cur += sizeof(double);
            this.camera_positionZ = BitConverter.ToDouble(data, cur); cur += sizeof(double);
            this.camera_targetX = BitConverter.ToDouble(data, cur); cur += sizeof(double);
            this.camera_targetY = BitConverter.ToDouble(data, cur); cur += sizeof(double);
            this.camera_targetZ = BitConverter.ToDouble(data, cur); cur += sizeof(double);
            this.camera_upX = BitConverter.ToSingle(data, cur); cur += sizeof(float);
            this.camera_upY = BitConverter.ToSingle(data, cur); cur += sizeof(float);
            this.camera_upZ = BitConverter.ToSingle(data, cur); cur += sizeof(float);
            this.keyboard_move_speed0 = BitConverter.ToSingle(data, cur); cur += sizeof(float);
            this.keyboard_move_speed1 = BitConverter.ToSingle(data, cur); cur += sizeof(float);
            this.keyboard_zoom_speed0 = BitConverter.ToSingle(data, cur); cur += sizeof(float);
            this.keyboard_zoom_speed1 = BitConverter.ToSingle(data, cur); cur += sizeof(float);
        }

        public void FromBytes3(byte[] data, double g)
        {
            this.g = g;
            ushort bools = (ushort)(((ushort)data[13] << 8) + (ushort)data[12]);
            int cur = 0;
            this.two_dimensional_calculation = ((bools >> cur++) & 1) == 1 ? true : false;
            this.two_dimensional_graphic = ((bools >> cur++) & 1) == 1 ? true : false;
            this.two_dimensional_binary = ((bools >> cur++) & 1) == 1 ? true : false;
            this.fullscreen = ((bools >> cur++) & 1) == 1 ? true : false;
            this.clear_screen = ((bools >> cur++) & 1) == 1 ? true : false;
            this.show_trails = ((bools >> cur++) & 1) == 1 ? true : false;
            this.min_text = ((bools >> cur++) & 1) == 1 ? true : false;
            this.crosshair = ((bools >> cur++) & 1) == 1 ? true : false;
            this.wireframe = ((bools >> cur++) & 1) == 1 ? true : false;
            this.paused = ((bools >> cur++) & 1) == 1 ? true : false;
            this.log = ((bools >> cur++) & 1) == 1 ? true : false;
            this.opencl = ((bools >> cur++) & 1) == 1 ? true : false;

            cur = 14;
            this.width = BitConverter.ToUInt32(data, cur); cur += sizeof(uint);
            this.height = BitConverter.ToUInt32(data, cur); cur += sizeof(uint);
            this.algorithm = (AlgorithmType)data[cur]; cur += sizeof(byte);
            this.dt = BitConverter.ToDouble(data, cur); cur += sizeof(double);
            this.graphic_max_rate = BitConverter.ToSingle(data, cur); cur += sizeof(float);
            this.binary_max_rate = BitConverter.ToSingle(data, cur); cur += sizeof(float);
            this.max_calculations = BitConverter.ToInt64(data, cur); cur += sizeof(long);
            this.max_trails = BitConverter.ToUInt32(data, cur); cur += sizeof(int);
            this.stick_to_body = BitConverter.ToUInt32(data, cur); cur += sizeof(uint);
            this.sphere_slices = BitConverter.ToUInt32(data, cur); cur += sizeof(uint);
            this.sphere_stacks = BitConverter.ToUInt32(data, cur); cur += sizeof(uint);
            this.field_of_view = BitConverter.ToSingle(data, cur); cur += sizeof(float);
            this.near_plane_distance = BitConverter.ToDouble(data, cur); cur += sizeof(double);
            this.far_plane_distance = BitConverter.ToDouble(data, cur); cur += sizeof(double);
            this.camera_positionX = BitConverter.ToDouble(data, cur); cur += sizeof(double);
            this.camera_positionY = BitConverter.ToDouble(data, cur); cur += sizeof(double);
            this.camera_positionZ = BitConverter.ToDouble(data, cur); cur += sizeof(double);
            this.camera_targetX = BitConverter.ToDouble(data, cur); cur += sizeof(double);
            this.camera_targetY = BitConverter.ToDouble(data, cur); cur += sizeof(double);
            this.camera_targetZ = BitConverter.ToDouble(data, cur); cur += sizeof(double);
            this.camera_upX = BitConverter.ToSingle(data, cur); cur += sizeof(float);
            this.camera_upY = BitConverter.ToSingle(data, cur); cur += sizeof(float);
            this.camera_upZ = BitConverter.ToSingle(data, cur); cur += sizeof(float);
            this.keyboard_move_speed0 = BitConverter.ToSingle(data, cur); cur += sizeof(float);
            this.keyboard_move_speed1 = BitConverter.ToSingle(data, cur); cur += sizeof(float);
            this.keyboard_zoom_speed0 = BitConverter.ToSingle(data, cur); cur += sizeof(float);
            this.keyboard_zoom_speed1 = BitConverter.ToSingle(data, cur); cur += sizeof(float);
            this.cl_num_of_threads = BitConverter.ToUInt32(data, cur); cur += sizeof(uint);
            this.cl_threads_in_workgroup = BitConverter.ToUInt32(data, cur); cur += sizeof(uint);
            this.cl_calcs_in_run = BitConverter.ToUInt32(data, cur); cur += sizeof(uint);
        }

        public bool FromBytes(byte[] data, double g)
        {
            uint ver = BitConverter.ToUInt32(data, 8);
            switch (ver)
            {
                case 1:
                    FromBytes1(data, g);
                    return false;
                case 2:
                    FromBytes2(data, g);
                    return false;
                default:
                    FromBytes3(data, g);
                    return true;
            }
        }

        public byte[] ToBytes()
        {
            ushort bools = 0;
            int cur = 0;
            bools |= (ushort)((this.two_dimensional_calculation ? 1 : 0) << cur++);
            bools |= (ushort)((this.two_dimensional_graphic ? 1 : 0) << cur++);
            bools |= (ushort)((this.two_dimensional_binary ? 1 : 0) << cur++);
            bools |= (ushort)((this.fullscreen ? 1 : 0) << cur++);
            bools |= (ushort)((this.clear_screen ? 1 : 0) << cur++);
            bools |= (ushort)((this.show_trails ? 1 : 0) << cur++);
            bools |= (ushort)((this.min_text ? 1 : 0) << cur++);
            bools |= (ushort)((this.crosshair ? 1 : 0) << cur++);
            bools |= (ushort)((this.wireframe ? 1 : 0) << cur++);
            bools |= (ushort)((this.paused ? 1 : 0) << cur++);
            bools |= (ushort)((this.log ? 1 : 0) << cur++);
            bools |= (ushort)((this.opencl ? 1 : 0) << cur++);

            List<byte> list = new List<byte>();
            list.AddRange(BitConverter.GetBytes((ulong)0xBDF0BDF01111BDF0));
            list.AddRange(BitConverter.GetBytes((uint)0x00000003));
            list.AddRange(BitConverter.GetBytes(bools));
            list.AddRange(BitConverter.GetBytes(this.width));
            list.AddRange(BitConverter.GetBytes(this.height));
            list.Add((byte)this.algorithm);
            list.AddRange(BitConverter.GetBytes(this.dt));
            list.AddRange(BitConverter.GetBytes(this.graphic_max_rate));
            list.AddRange(BitConverter.GetBytes(this.binary_max_rate));
            list.AddRange(BitConverter.GetBytes(this.max_calculations));
            list.AddRange(BitConverter.GetBytes(this.max_trails));
            list.AddRange(BitConverter.GetBytes(this.stick_to_body));
            list.AddRange(BitConverter.GetBytes(this.sphere_slices));
            list.AddRange(BitConverter.GetBytes(this.sphere_stacks));
            list.AddRange(BitConverter.GetBytes(this.field_of_view));
            list.AddRange(BitConverter.GetBytes(this.near_plane_distance));
            list.AddRange(BitConverter.GetBytes(this.far_plane_distance));
            list.AddRange(BitConverter.GetBytes(this.camera_positionX));
            list.AddRange(BitConverter.GetBytes(this.camera_positionY));
            list.AddRange(BitConverter.GetBytes(this.camera_positionZ));
            list.AddRange(BitConverter.GetBytes(this.camera_targetX));
            list.AddRange(BitConverter.GetBytes(this.camera_targetY));
            list.AddRange(BitConverter.GetBytes(this.camera_targetZ));
            list.AddRange(BitConverter.GetBytes(this.camera_upX));
            list.AddRange(BitConverter.GetBytes(this.camera_upY));
            list.AddRange(BitConverter.GetBytes(this.camera_upZ));
            list.AddRange(BitConverter.GetBytes(this.keyboard_move_speed0));
            list.AddRange(BitConverter.GetBytes(this.keyboard_move_speed1));
            list.AddRange(BitConverter.GetBytes(this.keyboard_zoom_speed0));
            list.AddRange(BitConverter.GetBytes(this.keyboard_zoom_speed1));
            list.AddRange(BitConverter.GetBytes(this.cl_num_of_threads));
            list.AddRange(BitConverter.GetBytes(this.cl_threads_in_workgroup));
            list.AddRange(BitConverter.GetBytes(this.cl_calcs_in_run));
            return list.ToArray();
        }
    }
    public class Body3D
    {
        private const int byte_size = 9 * sizeof(double) + 2 * 4 * sizeof(byte);

        private string name;
        private double positionX;
        private double positionY;
        private double positionZ;
        private double velocityX;
        private double velocityY;
        private double velocityZ;
        private double mass;
        private double radius;
        private double trailwidth;
        private Color color;
        private Color trailcolor;

        public Body3D()
        {
            this.name = "";
            this.positionX = 0;
            this.positionY = 0;
            this.positionZ = 0;
            this.velocityX = 0;
            this.velocityY = 0;
            this.velocityZ = 0;
            this.mass = 0;
            this.radius = 0;
            this.trailwidth = 0;
            this.color = Color.Transparent;
            this.trailcolor = Color.Transparent;
        }

        [DescriptionAttribute("The Name of the body.")]
        public string Name
        {
            get { return this.name; }
            set { this.name = value; }
        }

        [DescriptionAttribute("The X value in meters of the body's position.")]
        public double PositionX
        {
            get { return this.positionX; }
            set { this.positionX = value; }
        }

        [DescriptionAttribute("The Y value in meters of the body's position")]
        public double PositionY
        {
            get { return this.positionY; }
            set { this.positionY = value; }
        }

        [DescriptionAttribute("The Z value in meters of the body's position.")]
        public double PositionZ
        {
            get { return this.positionZ; }
            set { this.positionZ = value; }
        }

        [DescriptionAttribute("The X value in meters per second of the body's volcitiy.")]
        public double VelocityX
        {
            get { return this.velocityX; }
            set { this.velocityX = value; }
        }

        [DescriptionAttribute("The Y value in meters per second of the body's velocity.")]
        public double VelocityY
        {
            get { return this.velocityY; }
            set { this.velocityY = value; }
        }

        [DescriptionAttribute("The Z value in meters per second of the body's velocity.")]
        public double VelocityZ
        {
            get { return this.velocityZ; }
            set { this.velocityZ = value; }
        }

        [DescriptionAttribute("The body's mass in kilograms.")]
        public double Mass
        {
            get { return this.mass; }
            set { this.mass = value; }
        }

        [DescriptionAttribute("The body's radius in meters.")]
        public double Radius
        {
            get { return this.radius; }
            set { this.radius = value; }
        }

        [DescriptionAttribute("The width in meters of the bodie's trail.")]
        public double TrailWidth
        {
            get { return this.trailwidth; }
            set { this.trailwidth = value; }
        }

        [DescriptionAttribute("The color of the body.")]
        public Color Color
        {
            get { return this.color; }
            set { this.color = value; }
        }

        [DescriptionAttribute("The color of the body's trail.")]
        public Color TrailColor
        {
            get { return this.trailcolor; }
            set { this.trailcolor = value; }
        }

        private void FromBytes0(byte[] data, int byte_index)
        {
            int cur = byte_index;
            this.positionX = BitConverter.ToDouble(data, cur); cur += sizeof(double);
            this.positionY = BitConverter.ToDouble(data, cur); cur += sizeof(double);
            this.positionZ = BitConverter.ToDouble(data, cur); cur += sizeof(double);
            this.velocityX = BitConverter.ToDouble(data, cur); cur += sizeof(double);
            this.velocityY = BitConverter.ToDouble(data, cur); cur += sizeof(double);
            this.velocityZ = BitConverter.ToDouble(data, cur); cur += sizeof(double);
            this.mass = BitConverter.ToDouble(data, cur); cur += sizeof(double);
            this.radius = BitConverter.ToDouble(data, cur); cur += sizeof(double);
            this.trailwidth = BitConverter.ToDouble(data, cur); cur += sizeof(double);
            this.color = Color.FromArgb((int)data[cur + 3], (int)data[cur + 0], (int)data[cur + 1], (int)data[cur + 2]); cur += 4 * sizeof(byte);
            this.trailcolor = Color.FromArgb((int)data[cur + 3], (int)data[cur + 0], (int)data[cur + 1], (int)data[cur + 2]); cur += 4 * sizeof(byte);
        }

        private void FromBytes1(byte[] data, int byte_index)
        {
            int cur = byte_index;
            this.positionX = BitConverter.ToDouble(data, cur); cur += sizeof(double);
            this.positionY = BitConverter.ToDouble(data, cur); cur += sizeof(double);
            this.positionZ = BitConverter.ToDouble(data, cur); cur += sizeof(double);
            this.velocityX = BitConverter.ToDouble(data, cur); cur += sizeof(double);
            this.velocityY = BitConverter.ToDouble(data, cur); cur += sizeof(double);
            this.velocityZ = BitConverter.ToDouble(data, cur); cur += sizeof(double);
            this.mass = BitConverter.ToDouble(data, cur); cur += sizeof(double);
            this.radius = BitConverter.ToDouble(data, cur); cur += sizeof(double);
            this.trailwidth = BitConverter.ToDouble(data, cur); cur += sizeof(double);
            this.color = Color.FromArgb((int)data[cur + 3], (int)data[cur + 0], (int)data[cur + 1], (int)data[cur + 2]); cur += 4 * sizeof(byte);
            this.trailcolor = Color.FromArgb((int)data[cur + 3], (int)data[cur + 0], (int)data[cur + 1], (int)data[cur + 2]); cur += 4 * sizeof(byte);
        }

        private void FromBytes2(byte[] data, int byte_index)
        {
            int cur = byte_index;
            this.positionX = BitConverter.ToDouble(data, cur); cur += sizeof(double);
            this.positionY = BitConverter.ToDouble(data, cur); cur += sizeof(double);
            this.positionZ = BitConverter.ToDouble(data, cur); cur += sizeof(double);
            this.velocityX = BitConverter.ToDouble(data, cur); cur += sizeof(double);
            this.velocityY = BitConverter.ToDouble(data, cur); cur += sizeof(double);
            this.velocityZ = BitConverter.ToDouble(data, cur); cur += sizeof(double);
            this.mass = BitConverter.ToDouble(data, cur); cur += sizeof(double);
            this.radius = BitConverter.ToDouble(data, cur); cur += sizeof(double);
            this.trailwidth = BitConverter.ToDouble(data, cur); cur += sizeof(double);
            this.color = Color.FromArgb((int)data[cur + 3], (int)data[cur + 0], (int)data[cur + 1], (int)data[cur + 2]); cur += 4 * sizeof(byte);
            this.trailcolor = Color.FromArgb((int)data[cur + 3], (int)data[cur + 0], (int)data[cur + 1], (int)data[cur + 2]); cur += 4 * sizeof(byte);
        }

        private void FromBytes3(byte[] data, int byte_index)
        {
            int cur = byte_index;
            this.positionX = BitConverter.ToDouble(data, cur); cur += sizeof(double);
            this.positionY = BitConverter.ToDouble(data, cur); cur += sizeof(double);
            this.positionZ = BitConverter.ToDouble(data, cur); cur += sizeof(double);
            this.velocityX = BitConverter.ToDouble(data, cur); cur += sizeof(double);
            this.velocityY = BitConverter.ToDouble(data, cur); cur += sizeof(double);
            this.velocityZ = BitConverter.ToDouble(data, cur); cur += sizeof(double);
            this.mass = BitConverter.ToDouble(data, cur); cur += sizeof(double);
            this.radius = BitConverter.ToDouble(data, cur); cur += sizeof(double);
            this.trailwidth = BitConverter.ToDouble(data, cur); cur += sizeof(double);
            this.color = Color.FromArgb((int)data[cur + 3], (int)data[cur + 0], (int)data[cur + 1], (int)data[cur + 2]); cur += 4 * sizeof(byte);
            this.trailcolor = Color.FromArgb((int)data[cur + 3], (int)data[cur + 0], (int)data[cur + 1], (int)data[cur + 2]); cur += 4 * sizeof(byte);
        }

        private List<byte> ToBytes()
        {
            List<byte> list = new List<byte>(byte_size);
            list.AddRange(BitConverter.GetBytes(this.positionX));
            list.AddRange(BitConverter.GetBytes(this.positionY));
            list.AddRange(BitConverter.GetBytes(this.positionZ));
            list.AddRange(BitConverter.GetBytes(this.velocityX));
            list.AddRange(BitConverter.GetBytes(this.velocityY));
            list.AddRange(BitConverter.GetBytes(this.velocityZ));
            list.AddRange(BitConverter.GetBytes(this.mass));
            list.AddRange(BitConverter.GetBytes(this.radius));
            list.AddRange(BitConverter.GetBytes(this.trailwidth));
            list.Add(this.color.R);
            list.Add(this.color.G);
            list.Add(this.color.B);
            list.Add(this.color.A);
            list.Add(this.trailcolor.R);
            list.Add(this.trailcolor.G);
            list.Add(this.trailcolor.B);
            list.Add(this.trailcolor.A);
            return list;
        }

        public static List<Body3D> ListFromBytes0(byte[] data)
        {
            List<Body3D> list = new List<Body3D>(BitConverter.ToInt32(data, sizeof(double)));
            for (int i = 0; i < list.Capacity; i++)
            {
                list.Add(new Body3D());
                list[i].FromBytes0(data, byte_size * i + (sizeof(int) + sizeof(double)));
            }

            char[] names = new char[data.Length - list.Count * byte_size - (sizeof(int) + sizeof(double))];
            for (int i = list.Count * byte_size + (sizeof(int) + sizeof(double)); i < data.Length; i++)
                names[i - list.Count * byte_size - (sizeof(int) + sizeof(double))] = (char)data[i];
            string[] names_s = new string(names).Split('\0');
            for (int i = 0; i < list.Count; i++)
                list[i].Name = names_s[i];

            return list;
        }

        public static List<Body3D> ListFromBytes1(byte[] data)
        {
            List<Body3D> list = new List<Body3D>(BitConverter.ToInt32(data, sizeof(ulong) + sizeof(uint) + sizeof(double)));
            for (int i = 0; i < list.Capacity; i++)
            {
                list.Add(new Body3D());
                list[i].FromBytes1(data, byte_size * i + (sizeof(ulong) + sizeof(uint) + sizeof(int) + sizeof(double)));
            }

            char[] names = new char[data.Length - list.Count * byte_size - (sizeof(ulong) + sizeof(uint) + sizeof(int) + sizeof(double))];
            for (int i = list.Count * byte_size + (sizeof(ulong) + sizeof(uint) + sizeof(int) + sizeof(double)); i < data.Length; i++)
                names[i - list.Count * byte_size - (sizeof(ulong) + sizeof(uint) + sizeof(int) + sizeof(double))] = (char)data[i];
            string[] names_s = new string(names).Split('\0');
            for (int i = 0; i < list.Count; i++)
                list[i].Name = names_s[i];

            return list;
        }

        public static List<Body3D> ListFromBytes2(byte[] data)
        {
            List<Body3D> list = new List<Body3D>(BitConverter.ToInt32(data, sizeof(ulong) + sizeof(uint) + sizeof(double)));
            for (int i = 0; i < list.Capacity; i++)
            {
                list.Add(new Body3D());
                list[i].FromBytes2(data, byte_size * i + (sizeof(ulong) + sizeof(uint) + sizeof(int) + sizeof(double)));
            }

            char[] names = new char[data.Length - list.Count * byte_size - (sizeof(ulong) + sizeof(uint) + sizeof(int) + sizeof(double))];
            for (int i = list.Count * byte_size + (sizeof(ulong) + sizeof(uint) + sizeof(int) + sizeof(double)); i < data.Length; i++)
                names[i - list.Count * byte_size - (sizeof(ulong) + sizeof(uint) + sizeof(int) + sizeof(double))] = (char)data[i];
            string[] names_s = new string(names).Split('\0');
            for (int i = 0; i < list.Count; i++)
                list[i].Name = names_s[i];

            return list;
        }

        public static List<Body3D> ListFromBytes3(byte[] data)
        {
            List<Body3D> list = new List<Body3D>(BitConverter.ToInt32(data, sizeof(ulong) + sizeof(uint) + sizeof(double)));
            for (int i = 0; i < list.Capacity; i++)
            {
                list.Add(new Body3D());
                list[i].FromBytes3(data, byte_size * i + (sizeof(ulong) + sizeof(uint) + sizeof(int) + sizeof(double)));
            }

            char[] names = new char[data.Length - list.Count * byte_size - (sizeof(ulong) + sizeof(uint) + sizeof(int) + sizeof(double))];
            for (int i = list.Count * byte_size + (sizeof(ulong) + sizeof(uint) + sizeof(int) + sizeof(double)); i < data.Length; i++)
                names[i - list.Count * byte_size - (sizeof(ulong) + sizeof(uint) + sizeof(int) + sizeof(double))] = (char)data[i];
            string[] names_s = new string(names).Split('\0');
            for (int i = 0; i < list.Count; i++)
                list[i].Name = names_s[i];

            return list;
        }

        public static List<Body3D> ListFromBytes(byte[] data, out bool ret)
        {
            uint ver = BitConverter.ToUInt32(data, 8);
            switch (ver)
            {
                case 1:
                    ret = false;
                    return ListFromBytes1(data);
                case 2:
                    ret = false;
                    return ListFromBytes2(data);
                default:
                    ret = true;
                    return ListFromBytes3(data);
            }
        }

        public static byte[] ListToBytes(List<Body3D> list, double g)
        {
            List<byte> bytes = new List<byte>((byte_size + 5) * list.Count + (sizeof(ulong) + sizeof(uint) + sizeof(int) + sizeof(double)));
            bytes.AddRange(BitConverter.GetBytes((ulong)0xBDF0BDF02222BDF0));
            bytes.AddRange(BitConverter.GetBytes((uint)0x00000003));
            bytes.AddRange(BitConverter.GetBytes(g));
            bytes.AddRange(BitConverter.GetBytes(list.Count));
            for (int i = 0; i < list.Count; i++)
                bytes.AddRange(list[i].ToBytes());

            List<char> chars = new List<char>(5 * list.Count);
            for (int i = 0; i < list.Count; i++)
            {
                chars.AddRange(list[i].Name.ToCharArray());
                chars.Add('\0');
            }
            for (int i = 0; i < chars.Count; i++)
                bytes.Add((byte)chars[i]);

            return bytes.ToArray();
        }
    }
}