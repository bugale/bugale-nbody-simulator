using System.ComponentModel;
using System.Drawing;
using System;
using System.Collections.Generic;

public enum AlgorithmType { Leapfrog = 0, Hermite = 1 }
public class MainSettings
{
    private int width;
    private int height;
    private bool two_dimensional_calculation;
    private bool two_dimensional_graphic;
    private bool two_dimensional_binary;
    private bool fullscreen;
    private bool log;
    private AlgorithmType algorithm;
    private double g;
    private double dt;
    private double graphic_max_rate;
    private double binary_max_rate;
    private long max_calculations;
    int max_trails;
    private int sphere_slices;
    private int sphere_stacks;
    private double field_of_view;
    private double near_plane_distance;
    private double far_plane_distance;
    private double camera_positionX;
    private double camera_positionY;
    private double camera_positionZ;
    private double camera_targetX;
    private double camera_targetY;
    private double camera_targetZ;
    private double camera_upX;
    private double camera_upY;
    private double camera_upZ;
    private double keyboard_move_speed0;
    private double keyboard_move_speed1;
    private double keyboard_zoom_speed0;
    private double keyboard_zoom_speed1;

    public MainSettings()
    {
        this.width = 0;
        this.height = 0;
        this.two_dimensional_calculation = false;
        this.two_dimensional_graphic = false;
        this.two_dimensional_binary = false;
        this.fullscreen = false;
        this.log = false;
        this.algorithm = AlgorithmType.Leapfrog;
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
    }

    [DescriptionAttribute("The width in pixels of the screen.")]
    public int Width
    {
        get { return this.width; }
        set { this.width = value; }
    }

    [DescriptionAttribute("The height in pixels of the screen.")]
    public int Height
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

    [DescriptionAttribute("Determines whether the logging eill be enabled or not.")]
    public bool Log
    {
        get { return this.log; }
        set { this.log = value; }
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
    public double Dt
    {
        get { return this.dt; }
        set { this.dt = value; }
    }

    [DescriptionAttribute("The maximal rate in s^-1 of the graphical output update. Write 0 for disabling graphical output, Write a negative number for V-Sync.")]
    public double GraphicMaxRate
    {
        get { return this.graphic_max_rate; }
        set { this.graphic_max_rate = value; }
    }

    [DescriptionAttribute("The maximal rate in s^-1 of the binary output update. Write 0 for disabling textual output, write a negative number for updating as soon as possible.")]
    public double BinaryMaxRate
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
    public int MaxTrails
    {
        get { return this.max_trails; }
        set { this.max_trails = value; }
    }

    [DescriptionAttribute("The number of slices in the rendered spheres. More slices = better looking spheres.")]
    public int SphereSlices
    {
        get { return this.sphere_slices; }
        set { this.sphere_slices = value; }
    }

    [DescriptionAttribute("The number of stacks in the rendered spheres. More stacks = better looking spheres.")]
    public int SphereStacks
    {
        get { return this.sphere_stacks; }
        set { this.sphere_stacks = value; }
    }

    [DescriptionAttribute("The horizontal field of view in angles.")]
    public double FieldOfView
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
    public double CameraUpX
    {
        get { return this.camera_upX; }
        set { this.camera_upX = value; }
    }

    [DescriptionAttribute("The Y value of the up vector of the camera. Used only in 3D graphical output.")]
    public double CameraUpY
    {
        get { return this.camera_upY; }
        set { this.camera_upY = value; }
    }

    [DescriptionAttribute("The Z value of the up vector of the camera. Used only in 3D graphical output.")]
    public double CameraUpZ
    {
        get { return this.camera_upZ; }
        set { this.camera_upZ = value; }
    }

    [DescriptionAttribute("The initial movement speed of the camera(2D)/target(3D) when using the keyboard, in screens per second.")]
    public double KeyboardMoveSpeed0
    {
        get { return this.keyboard_move_speed0; }
        set { this.keyboard_move_speed0 = value; }
    }

    [DescriptionAttribute("The movement speed of the camera(2D)/target(3D) after one second when using the keyboard, in screens per second.")]
    public double KeyboardMoveSpeed1
    {
        get { return this.keyboard_move_speed1; }
        set { this.keyboard_move_speed1 = value; }
    }

    [DescriptionAttribute("The initial zoom speed of the camera.")]
    public double KeyboardZoomSpeed0
    {
        get { return this.keyboard_zoom_speed0; }
        set { this.keyboard_zoom_speed0 = value; }
    }

    [DescriptionAttribute("The zoom speed of the camera after one second.")]
    public double KeyboardZoomSpeed1
    {
        get { return this.keyboard_zoom_speed1; }
        set { this.keyboard_zoom_speed1 = value; }
    }

    public void FromBytes(byte[] data, double g)
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
        this.width = BitConverter.ToInt32(data, cur); cur += sizeof(int);
        this.height = BitConverter.ToInt32(data, cur); cur += sizeof(int);
        this.algorithm = (AlgorithmType)data[cur]; cur += sizeof(byte);
        this.dt = BitConverter.ToDouble(data, cur); cur += sizeof(double);
        this.graphic_max_rate = BitConverter.ToDouble(data, cur); cur += sizeof(double);
        this.binary_max_rate = BitConverter.ToDouble(data, cur); cur += sizeof(double);
        this.max_calculations = BitConverter.ToInt64(data, cur); cur += sizeof(long);
        this.max_trails = BitConverter.ToInt32(data, cur); cur += sizeof(int);
        this.sphere_slices = BitConverter.ToInt32(data, cur); cur += sizeof(int);
        this.sphere_stacks = BitConverter.ToInt32(data, cur); cur += sizeof(int);
        this.field_of_view = BitConverter.ToDouble(data, cur); cur += sizeof(double);
        this.near_plane_distance = BitConverter.ToDouble(data, cur); cur += sizeof(double);
        this.far_plane_distance = BitConverter.ToDouble(data, cur); cur += sizeof(double);
        this.camera_positionX = BitConverter.ToDouble(data, cur); cur += sizeof(double);
        this.camera_positionY = BitConverter.ToDouble(data, cur); cur += sizeof(double);
        this.camera_positionZ = BitConverter.ToDouble(data, cur); cur += sizeof(double);
        this.camera_targetX = BitConverter.ToDouble(data, cur); cur += sizeof(double);
        this.camera_targetY = BitConverter.ToDouble(data, cur); cur += sizeof(double);
        this.camera_targetZ = BitConverter.ToDouble(data, cur); cur += sizeof(double);
        this.camera_upX = BitConverter.ToDouble(data, cur); cur += sizeof(double);
        this.camera_upY = BitConverter.ToDouble(data, cur); cur += sizeof(double);
        this.camera_upZ = BitConverter.ToDouble(data, cur); cur += sizeof(double);
        this.keyboard_move_speed0 = BitConverter.ToDouble(data, cur); cur += sizeof(double);
        this.keyboard_move_speed1 = BitConverter.ToDouble(data, cur); cur += sizeof(double);
        this.keyboard_zoom_speed0 = BitConverter.ToDouble(data, cur); cur += sizeof(double);
        this.keyboard_zoom_speed1 = BitConverter.ToDouble(data, cur); cur += sizeof(double);
    }

    public byte[] ToBytes()
    {
        byte bools = 0;
        int cur = 0;
        bools |= (byte)((this.two_dimensional_calculation ? 1 : 0) << cur++);
        bools |= (byte)((this.two_dimensional_graphic ? 1 : 0) << cur++);
        bools |= (byte)((this.two_dimensional_binary ? 1 : 0) << cur++);
        bools |= (byte)((this.fullscreen ? 1 : 0) << cur++);
        bools |= (byte)((this.log ? 1 : 0) << cur++);

        List<byte> list = new List<byte>();
        list.Add(bools);
        list.AddRange(BitConverter.GetBytes(this.width));
        list.AddRange(BitConverter.GetBytes(this.height));
        list.Add     (                (byte)this.algorithm);
        list.AddRange(BitConverter.GetBytes(this.dt));
        list.AddRange(BitConverter.GetBytes(this.graphic_max_rate));
        list.AddRange(BitConverter.GetBytes(this.binary_max_rate));
        list.AddRange(BitConverter.GetBytes(this.max_calculations));
        list.AddRange(BitConverter.GetBytes(this.max_trails));
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

    private void FromBytes(byte[] data, int byte_index)
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

    public static List<Body3D> ListFromBytes(byte[] data)
    {
        List<Body3D> list = new List<Body3D>(BitConverter.ToInt32(data, sizeof(double)));
        for (int i = 0; i < list.Capacity; i++)
        {
            list.Add(new Body3D());
            list[i].FromBytes(data, byte_size * i + (sizeof(int) + sizeof(double)));
        }

        char[] names = new char[data.Length - list.Count * byte_size - (sizeof(int) + sizeof(double))];
        for (int i = list.Count * byte_size + (sizeof(int) + sizeof(double)); i < data.Length; i++)
            names[i - list.Count * byte_size - (sizeof(int) + sizeof(double))] = (char)data[i];
        string[] names_s = new string(names).Split('\0');
        for (int i = 0; i < list.Count; i++)
            list[i].Name = names_s[i];

        return list;
    }

    public static byte[] ListToBytes(List<Body3D> list, double g)
    {
        List<byte> bytes = new List<byte>((byte_size + 5) * list.Count + (sizeof(int) + sizeof(double)));
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
