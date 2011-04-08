using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Diagnostics;
using SdlDotNet.Core;
using SdlDotNet.Graphics;
using System.Drawing;
using SdlDotNet.Input;
using System.Windows.Forms;
using System.Xml;
using SdlDotNet.Graphics.Primitives;
using System.Threading;
using System.Runtime.InteropServices;
using NBodySimulator.Properties;

namespace Starter
{
    class Starter
    {
        [STAThread]
        public static void Main()
        {
            new Simulator.Simulator();
        }
    }
}
namespace Simulator
{
    class Simulator
    {
        [DllImport("kernel32.dll")] static extern bool AllocConsole();

        //Fields
        #region Software
            int version = 10;
            bool speed_tester = false;
            ulong speed_tester_rounds = 0;
            int max_version;
            bool two_dimensional = true;
        #endregion
        #region Graphics
            #region Properties
                Surface screen;
                int screen_width = 1024;
                int screen_height = 768;
                double screen_width_meters = 1024;
                double screen_height_meters = 768;
                int ave_fps = 1; //Average frames per second
                double cur_fps = 0; //Frames per second in the last second
                double cur_cps = 0; //Calculations per second in the last second
                ulong done_calculations = 0;
                ulong done_frames = 0;
                ulong done_calculations_lastsecond = 0;
                ulong done_frames_lastsecond = 0;
                List<string> onscreen_lines = new List<string>();
                List<Color> onscreen_lines_colors = new List<Color>();
                double spc = 0; //Seconds per capture
                ulong num_of_captured = 0;
            #endregion
            #region Camera
                double original_mpp = 1;
                double original_X = 0;
                double original_Y = 0;
                double temp_mpp = 1;
                double temp_X = 0;
                double temp_Y = 0;
                double cur_mpp = 1;
                double cur_X = 0;
                double cur_Y = 0;
                double mouseX = 0;
                double mouseY = 0;
            #endregion
            #region Misc
                List<TrailDot> dots = new List<TrailDot>();
                VisualProp[] visual_props;
                int maxdots = 100000;
                int timeout = 0;
            #endregion
        #endregion
        #region Calculations
            Body2D[] bodies2D;
            Body3D[] bodies3D;
            int num_of_bodies;
            int num_of_bodies_minus1;
            double[,] g_m1_m2; // G * M1 * M2
            double[] dt_div_mass; // dt / M
            double[] mass_inverse; // 1 / M
            double[] mass_mul_g; // G * M
            double dt = 1;
            double dt_div_2 = 0.5; // dt / 2
            double dt2_div_2 = 0.5; // dt^2 / 2
            double g = 6.67428E-11;
            double startEner = 0;
            double curEner = 0;
        #endregion
        #region CameraChange
            Stopwatch pressed_horizontal = new Stopwatch();
            Stopwatch pressed_vertical = new Stopwatch();
            Stopwatch pressed_zoom = new Stopwatch();
            bool right_pressed = false;
            bool left_pressed = false;
            bool down_pressed = false;
            bool up_pressed = false;
            bool plus_pressed = false;
            bool minus_pressed = false;
        #endregion
        #region Options
            bool pause = false;
            bool show_trail = true;
            bool save_trail = true;
            bool refresh_screen = true;
            bool fullscreen = false;
            bool capture_screen = false;
        #endregion
        #region Timing
            bool activate_dot_thread = false;
            bool quit_threads = false;
            Stopwatch time_past = new Stopwatch();
            Stopwatch time_past_since_capture = new Stopwatch();
            List<Thread> threads = new List<Thread>();
        #endregion

        public Simulator()
        {
            this.max_version = this.version;
            this.Initialize();
        }
        public void Initialize()
        {
            this.InitializeData();
            this.InitializeBodyData();
            if (this.speed_tester) this.RunSpeedTester();
            else
            {
                this.InitializeVideo();
                this.InitializeEvents();
                this.InitializeThreads();
                this.InitializeTimers();
                this.startEner = this.curEner = this.GetEner();
                Events.Run();
            }
        }
        #region Initialization Helper
            private void RunSpeedTester()
            {
                AllocConsole();
                Console.Title = "N-Body Simulator Speed Tester";
                Console.WriteLine("Testing Speed for V0 - V" + this.max_version);
                Console.WriteLine("with " + this.speed_tester_rounds + " calculations");
                Console.WriteLine("on " + this.num_of_bodies + " bodies:");
                List<TimeSpan> timings2D = this.GenerateTimings(true);
                List<TimeSpan> timings3D = this.GenerateTimings(false);
                Console.WriteLine("Calculation Done!");
                string log2D = "2D NBody Simulation Logger, V0 - V" + this.max_version + " with " + this.speed_tester_rounds + " calculations on " + this.num_of_bodies + " bodies:\r\n\r\n";
                string log3D = "3D NBody Simulation Logger, V0 - V" + this.max_version + " with " + this.speed_tester_rounds + " calculations on " + this.num_of_bodies + " bodies:\r\n\r\n";
                log2D += this.GenerateTimingsLog(timings2D) + "\r\n\r\n";
                log3D += this.GenerateTimingsLog(timings3D) + "\r\n\r\n";
                log2D += this.GenerateCPSLog(timings2D);
                log3D += this.GenerateCPSLog(timings3D);
                Console.WriteLine("Log Generation Done!");
                System.IO.File.WriteAllText(Application.StartupPath + "\\2D.log", log2D);
                System.IO.File.WriteAllText(Application.StartupPath + "\\3D.log", log3D);
                Console.WriteLine("Log Saving Done!");
                Console.Write("Close this window now...");
                Console.ReadLine();
            }
            private void InitializeEvents()
            {
                Events.Quit += new EventHandler<QuitEventArgs>(this.Quitting);
                Events.Tick += new EventHandler<TickEventArgs>(this.Refresh);
                Events.KeyboardDown += new EventHandler<KeyboardEventArgs>(this.KeyDown);
                Events.KeyboardUp += new EventHandler<KeyboardEventArgs>(this.KeyUp);
                Events.MouseMotion += new EventHandler<MouseMotionEventArgs>(this.MouseMove);
            }
            private void InitializeThreads()
            {
                this.threads.Add(new Thread(new ThreadStart(AddDots)));
                if (timeout == 0) this.threads.Add(new Thread(new ThreadStart(Calc)));
                else this.threads.Add(new Thread(new ThreadStart(CalcWait)));
                this.threads.Add(new Thread(new ThreadStart(CalcSpeed)));

                //Start threads
                threads.ForEach(t => t.Start());
            }
            private void InitializeTimers()
            {
                this.time_past.Start();
                this.time_past_since_capture.Start();
            }
            private void InitializeData()
            {
                XmlDocument settings = new XmlDocument();
                XmlDocument bodies = new XmlDocument();
                settings.Load(Application.StartupPath + "\\Settings.xml");
                bodies.Load(Application.StartupPath + "\\Bodies.xml");
                for (int i = 0; i < settings.ChildNodes.Count; i++)
                    if (settings.ChildNodes[i].Name.ToLower() == "settings")
                    {
                        this.ReadSettings(settings.ChildNodes[i]);
                        break;
                    }
                for (int i = 0; i < bodies.ChildNodes.Count; i++)
                    if (bodies.ChildNodes[i].Name.ToLower() == "bodies")
                    {
                        this.ReadBodies(bodies.ChildNodes[i]);
                        break;
                    }
                this.cur_mpp = original_mpp;
                this.cur_X = this.original_X;
                this.cur_Y = this.original_Y;
                this.cur_fps = this.ave_fps;
            }
            private void InitializeBodyData()
            {
                this.dt_div_2 = this.dt / 2;
                this.dt2_div_2 = this.dt * this.dt / 2;
                this.num_of_bodies = this.bodies2D.Length;
                this.num_of_bodies_minus1 = this.bodies2D.Length - 1;

                this.dt_div_mass = new double[this.bodies2D.Length];
                this.mass_inverse = new double[this.bodies2D.Length];
                this.mass_mul_g = new double[this.bodies2D.Length];
                this.g_m1_m2 = new double[this.bodies2D.Length, this.bodies2D.Length];
                for (int i = 0; i < this.bodies2D.Length; i++)
                {
                    this.dt_div_mass[i] = this.dt / this.bodies2D[i]._mass;
                    this.mass_inverse[i] = 1 / this.bodies2D[i]._mass;
                    this.mass_mul_g[i] = this.g * this.bodies2D[i]._mass;
                }
                for (int i = 0; i < this.bodies2D.Length; i++)
                    for (int j = 0; j < this.bodies2D.Length; j++)
                        g_m1_m2[i, j] = g * this.bodies2D[i]._mass * this.bodies2D[j]._mass;
            }
            private void InitializeVideo()
            {
                Events.Fps = this.ave_fps;
                Video.WindowIcon(Resources.Icon);
                screen = Video.SetVideoMode(this.screen_width, this.screen_height, false, false, this.fullscreen);
                Video.WindowCaption = "Bugale L.T.D Corporations NBody Simulator";
            }
            private void ReadSettings(XmlNode settings)
            {
                for (int i = 0; i < settings.ChildNodes.Count; i++)
                {
                    if (settings.ChildNodes[i].InnerText.ToLower() != "default")
                    {
                        if (settings.ChildNodes[i].Name.ToLower() == "width") this.screen_width = int.Parse(settings.ChildNodes[i].InnerText);
                        if (settings.ChildNodes[i].Name.ToLower() == "height") this.screen_height = int.Parse(settings.ChildNodes[i].InnerText);
                        if (settings.ChildNodes[i].Name.ToLower() == "version") this.version = int.Parse(settings.ChildNodes[i].InnerText);
                        if (settings.ChildNodes[i].Name.ToLower() == "twodimensional") this.two_dimensional = bool.Parse(settings.ChildNodes[i].InnerText);
                        if (settings.ChildNodes[i].Name.ToLower() == "speedtester" || settings.ChildNodes[i].Name.ToLower() == "tester") this.speed_tester = bool.Parse(settings.ChildNodes[i].InnerText);
                        if (settings.ChildNodes[i].Name.ToLower() == "speedtesterrounds" || settings.ChildNodes[i].Name.ToLower() == "testerrounds") this.speed_tester_rounds = ulong.Parse(settings.ChildNodes[i].InnerText);
                        if (settings.ChildNodes[i].Name.ToLower() == "showtrail") this.show_trail = bool.Parse(settings.ChildNodes[i].InnerText);
                        if (settings.ChildNodes[i].Name.ToLower() == "savetrail") this.save_trail = bool.Parse(settings.ChildNodes[i].InnerText);
                        if (settings.ChildNodes[i].Name.ToLower() == "pause") this.pause = bool.Parse(settings.ChildNodes[i].InnerText);
                        if (settings.ChildNodes[i].Name.ToLower() == "refreshscreen") this.refresh_screen = bool.Parse(settings.ChildNodes[i].InnerText);
                        if (settings.ChildNodes[i].Name.ToLower() == "fullscreen") this.fullscreen = bool.Parse(settings.ChildNodes[i].InnerText);
                        if (settings.ChildNodes[i].Name.ToLower() == "timeout") this.timeout = int.Parse(settings.ChildNodes[i].InnerText);
                        if (settings.ChildNodes[i].Name.ToLower() == "maxtraildots") this.maxdots = int.Parse(settings.ChildNodes[i].InnerText);
                        if (settings.ChildNodes[i].Name.ToLower() == "capturescreen") this.capture_screen = bool.Parse(settings.ChildNodes[i].InnerText);
                        if (settings.ChildNodes[i].Name.ToLower() == "secondspercapture" || settings.ChildNodes[i].Name.ToLower() == "spc") this.spc = double.Parse(settings.ChildNodes[i].InnerText);
                        if (settings.ChildNodes[i].Name.ToLower() == "framespersecond" || settings.ChildNodes[i].Name.ToLower() == "fps") this.ave_fps = int.Parse(settings.ChildNodes[i].InnerText);
                        if (settings.ChildNodes[i].Name.ToLower() == "deltatime" || settings.ChildNodes[i].Name.ToLower() == "deltat" || settings.ChildNodes[i].Name.ToLower() == "dt") this.dt = double.Parse(settings.ChildNodes[i].InnerText);
                        if (settings.ChildNodes[i].Name.ToLower() == "g") this.g = double.Parse(settings.ChildNodes[i].InnerText);
                        if (settings.ChildNodes[i].Name.ToLower() == "mpp" || settings.ChildNodes[i].Name.ToLower() == "metersperpixel") this.original_mpp = double.Parse(settings.ChildNodes[i].InnerText);
                        if (settings.ChildNodes[i].Name.ToLower() == "x") this.original_X = double.Parse(settings.ChildNodes[i].InnerText);
                        if (settings.ChildNodes[i].Name.ToLower() == "y") this.original_Y = double.Parse(settings.ChildNodes[i].InnerText);
                    }
                }
            }
            private void ReadBodies(XmlNode bodies)
            {
                List<Body2D> body2DList = new List<Body2D>();
                List<Body3D> body3DList = new List<Body3D>();
                List<VisualProp> visualPropsList = new List<VisualProp>();
                for (int i = 0; i < bodies.ChildNodes.Count; i++)
                    AddBody(bodies.ChildNodes[i], body2DList, body3DList, visualPropsList);
                this.bodies2D = body2DList.ToArray();
                this.bodies3D = body3DList.ToArray();
                this.visual_props = visualPropsList.ToArray();
            }
            private void AddBody(XmlNode body, List<Body2D> body2DList, List<Body3D> body3DList, List<VisualProp> visualPropsList)
            {
                Body2D cur_body2D = new Body2D(0, 0, 0, 0, 0);
                Body3D cur_body3D = new Body3D(0, 0, 0, 0, 0, 0, 0);
                VisualProp cur_visual_prop = new VisualProp(0, 0, Color.Transparent, Color.Transparent);
                for (int j = 0; j < body.ChildNodes.Count; j++)
                {
                    if (body.ChildNodes[j].Name.ToLower() == "p" || body.ChildNodes[j].Name.ToLower() == "position" || body.ChildNodes[j].Name.ToLower() == "location" || body.ChildNodes[j].Name.ToLower() == "loc")
                    {
                        cur_body2D._positionX = double.Parse(body.ChildNodes[j].InnerText.Split(',', ';')[0]);
                        cur_body3D._positionX = double.Parse(body.ChildNodes[j].InnerText.Split(',', ';')[0]);
                        if (body.ChildNodes[j].InnerText.Split(',', ';').Length > 1) cur_body2D._positionY = double.Parse(body.ChildNodes[j].InnerText.Split(',', ';')[1]);
                        if (body.ChildNodes[j].InnerText.Split(',', ';').Length > 1) cur_body3D._positionY = double.Parse(body.ChildNodes[j].InnerText.Split(',', ';')[1]);
                        if (body.ChildNodes[j].InnerText.Split(',', ';').Length > 2) cur_body3D._positionZ = double.Parse(body.ChildNodes[j].InnerText.Split(',', ';')[2]);
                    }
                    if (body.ChildNodes[j].Name.ToLower() == "v" || body.ChildNodes[j].Name.ToLower() == "velocity" || body.ChildNodes[j].Name.ToLower() == "speed")
                    {
                        cur_body2D._velocityX = double.Parse(body.ChildNodes[j].InnerText.Split(',', ';')[0]);
                        cur_body3D._velocityX = double.Parse(body.ChildNodes[j].InnerText.Split(',', ';')[0]);
                        if (body.ChildNodes[j].InnerText.Split(',', ';').Length > 1) cur_body2D._velocityY = double.Parse(body.ChildNodes[j].InnerText.Split(',', ';')[1]);
                        if (body.ChildNodes[j].InnerText.Split(',', ';').Length > 1) cur_body3D._velocityY = double.Parse(body.ChildNodes[j].InnerText.Split(',', ';')[1]);
                        if (body.ChildNodes[j].InnerText.Split(',', ';').Length > 2) cur_body3D._velocityZ = double.Parse(body.ChildNodes[j].InnerText.Split(',', ';')[2]);
                    }
                    if (body.ChildNodes[j].Name.ToLower() == "m" || body.ChildNodes[j].Name.ToLower() == "mass")
                    {
                        cur_body2D._mass = double.Parse(body.ChildNodes[j].InnerText);
                        cur_body3D._mass = double.Parse(body.ChildNodes[j].InnerText);
                    }
                    if (body.ChildNodes[j].Name.ToLower() == "r" || body.ChildNodes[j].Name.ToLower() == "radius")
                        cur_visual_prop._radius = double.Parse(body.ChildNodes[j].InnerText);
                    if (body.ChildNodes[j].Name.ToLower() == "color")
                    {
                        cur_visual_prop._color = Color.FromArgb(int.Parse(body.ChildNodes[j].InnerText.Split(',', ';')[0]),
                            int.Parse(body.ChildNodes[j].InnerText.Split(',', ';')[1]),
                            int.Parse(body.ChildNodes[j].InnerText.Split(',', ';')[2]));
                    }
                    if (body.ChildNodes[j].Name.ToLower() == "trail" || body.ChildNodes[j].Name.ToLower() == "trailcolor")
                    {
                        cur_visual_prop._trail = Color.FromArgb(int.Parse(body.ChildNodes[j].InnerText.Split(',', ';')[0]),
                            int.Parse(body.ChildNodes[j].InnerText.Split(',', ';')[1]),
                            int.Parse(body.ChildNodes[j].InnerText.Split(',', ';')[2]));
                    }
                    if (body.ChildNodes[j].Name.ToLower() == "trailwidth") cur_visual_prop._trailradius = double.Parse(body.ChildNodes[j].InnerText);
                }
                body2DList.Add(cur_body2D);
                body3DList.Add(cur_body3D);
                visualPropsList.Add(cur_visual_prop);
            }
        #endregion
        #region Quit Section
            private void Quitting(object sender, QuitEventArgs e)
            {
                this.QuitApplication();
            }
            private void QuitThreads()
            {
                quit_threads = true;
                while (threads.Any(t => t.IsAlive)) { } //Wait until all threads are closed
            }
            private void QuitApplication()
            {
                this.QuitThreads();
                Events.QuitApplication();
            }
        #endregion
        #region Visual Drawing
            private void Refresh(object sender, TickEventArgs e)
            {
                this.activate_dot_thread = true;

                this.done_frames++;
                this.done_frames_lastsecond++;
                this.screen_width_meters = this.screen_width * this.cur_mpp;
                this.screen_height_meters = this.screen_height * this.cur_mpp;

                try { this.CaptureScreen(); } catch { }
                try { this.BlankScreen(); } catch { }
                try { this.ChangeCamera(); } catch { }
                try { this.DrawBodies(); } catch { }

                while (this.activate_dot_thread) { if (!this.save_trail || this.quit_threads) break; } //Wait

                try { if (this.show_trail) this.DrawTrails(); } catch { }
                try { GenerateOnscreenLines(); } catch { }
                try { this.screen.Update(); } catch { }
            }
            private void GenerateOnscreenLines()
            {
                this.onscreen_lines.Clear();
                this.onscreen_lines_colors.Clear();

                this.GenerateStatisticsOnscreenLines();
                this.GenerateSimulatedTimePastOnscreenLines();
                this.GenerateSecondsOnscreenLines();
                this.GenerateDataOnscreenLines();
                this.GenerateWarningsOnscreenLines();

                this.DrawOnscreenLines();
            }
            private void GenerateStatisticsOnscreenLines()
            {
                if (this.cur_cps >= 0) this.onscreen_lines.Add("Calculations per Second: ~" + ((ulong)this.cur_cps).ToString("#,##0"));
                else this.onscreen_lines.Add("Seconds per Calculation: ~" + ((ulong)(1 / ((double)this.done_calculations / this.time_past.Elapsed.TotalSeconds))).ToString("#,##0"));
                this.onscreen_lines_colors.Add(Color.White);

                if (this.cur_fps > 1) this.onscreen_lines.Add("Frames per Second: ~" + ((ulong)this.cur_fps).ToString("#,##0"));
                else this.onscreen_lines.Add("Seconds per Frame: ~" + ((ulong)(1 / ((double)this.done_frames / this.time_past.Elapsed.TotalSeconds))).ToString("#,##0"));
                this.onscreen_lines_colors.Add(Color.White);
            }
            private void GenerateSimulatedTimePastOnscreenLines()
            {
                if ((ulong)((double)this.done_calculations / this.time_past.Elapsed.TotalSeconds * this.dt) > 365.25 * 24 * 60 * 60 / 2)
                    this.onscreen_lines.Add("Simulated Years Past: " + ((ulong)((double)this.done_calculations * this.dt / 60 / 60 / 24 / 365.25)).ToString("#,##0"));
                else if ((double)this.done_calculations * this.dt / 60 / 60 / 24 / 365.25 > 1)
                    this.onscreen_lines.Add("Simulated Years Past: " + ((double)this.done_calculations * this.dt / 60 / 60 / 24 / 365.25).ToString());
                else if ((ulong)((double)this.done_calculations / this.time_past.Elapsed.TotalSeconds * this.dt) > 24 * 60 * 60 / 2)
                    this.onscreen_lines.Add("Simulated Days Past: " + ((ulong)((double)this.done_calculations * this.dt / 60 / 60 / 24)).ToString("#,##0"));
                else if ((double)this.done_calculations * this.dt / 60 / 60 / 24 > 1)
                    this.onscreen_lines.Add("Simulated Days Past: " + ((double)this.done_calculations * this.dt / 60 / 60 / 24).ToString());
                else if ((ulong)((double)this.done_calculations / this.time_past.Elapsed.TotalSeconds * this.dt) > 60 * 60 / 2)
                    this.onscreen_lines.Add("Simulated Hours Past: " + ((ulong)((double)this.done_calculations * this.dt / 60 / 60)).ToString("#,##0"));
                else if ((double)this.done_calculations * this.dt / 60 / 60 > 1) this.onscreen_lines.Add("Simulated Hours Past: " + ((double)this.done_calculations * dt / 60 / 60).ToString());
                else if ((ulong)((double)this.done_calculations / this.time_past.Elapsed.TotalSeconds * this.dt) > 60 / 2) onscreen_lines.Add("Simulated Minutes Past: " + ((ulong)((double)this.done_calculations * this.dt / 60)).ToString("#,##0"));
                else if ((double)done_calculations * this.dt / 60 > 1) 
                    this.onscreen_lines.Add("Simulated Minutes Past: " + ((double)this.done_calculations * this.dt / 60).ToString());
                else
                    this.onscreen_lines.Add("Simulated Seconds Past: " + ((ulong)((double)this.done_calculations * this.dt)).ToString("#,##0"));
                this.onscreen_lines_colors.Add(Color.White);
            }
            private void GenerateSecondsOnscreenLines()
            {
                this.onscreen_lines.Add("Simulated Seconds Past: " + String.Format("{0:n}", (double)this.done_calculations * this.dt));
                this.onscreen_lines_colors.Add(Color.White);

                this.onscreen_lines.Add("Real Seconds Past: " + ((ulong)(this.time_past.Elapsed.TotalSeconds)).ToString("#,##0"));
                this.onscreen_lines_colors.Add(Color.White);

                this.onscreen_lines.Add("Simulated - Real Time Ratio: " + ((double)this.done_calculations * dt).ToString("#,##0"));
                this.onscreen_lines_colors.Add(Color.White);
            }
            private void GenerateDataOnscreenLines()
            {
                this.onscreen_lines.Add("Number of bodies: " + this.num_of_bodies.ToString("#,##0"));
                this.onscreen_lines_colors.Add(Color.White);

                this.onscreen_lines.Add("G: " + g.ToString());
                this.onscreen_lines_colors.Add(Color.White);

                this.onscreen_lines.Add("DT: " + dt.ToString());
                this.onscreen_lines_colors.Add(Color.White);

                this.onscreen_lines.Add("Starting Energy: " + this.startEner.ToString("#.####e+0"));
                this.onscreen_lines_colors.Add(Color.White);

                this.onscreen_lines.Add("Current Energy: " + this.curEner.ToString("#.####e+0") + " (Error: " + (100 * (this.startEner - this.curEner) / this.startEner).ToString("0.####") + "%)");
                this.onscreen_lines_colors.Add(Color.White);
            }
            private void GenerateWarningsOnscreenLines()
            {
                if (this.pause)
                {
                    this.onscreen_lines.Add("Paused!");
                    this.onscreen_lines_colors.Add(Color.Red);
                }
                if (!this.show_trail)
                {
                    this.onscreen_lines.Add("Trails are Hidden!");
                    this.onscreen_lines_colors.Add(Color.Red);
                }
                if (!this.save_trail)
                {
                    this.onscreen_lines.Add("Trails are not Saved!");
                    this.onscreen_lines_colors.Add(Color.Red);
                }
                if (!this.refresh_screen)
                {
                    this.onscreen_lines.Add("Screen is not Being Cleaned!");
                    this.onscreen_lines_colors.Add(Color.Red);
                }
                if (this.capture_screen)
                {
                    this.onscreen_lines.Add("Screen is Being Captured!");
                    this.onscreen_lines_colors.Add(Color.Red);
                }
            }
            private void DrawOnscreenLines()
            {
                SdlDotNet.Graphics.Font font = new SdlDotNet.Graphics.Font("C:\\Windows\\Fonts\\arial.ttf", 12);

                for (int i = 0; i < onscreen_lines.Count; i++) this.screen.Blit(font.Render(this.onscreen_lines[i], this.onscreen_lines_colors[i]), new Point(0, 12 * i));

                double mouseXMeters = (this.mouseX - this.screen_width / 2) * this.cur_mpp + cur_X;
                double mouseYMeters = (this.screen_height / 2 - this.mouseY) * this.cur_mpp + cur_Y;

                if ((int)this.cur_mpp == 0) this.screen.Blit(font.Render("Meters Per Pixel: " + this.cur_mpp.ToString(), Color.White), new Point(0, this.screen_height - 86));
                else this.screen.Blit(font.Render("Meters Per Pixel: " + this.cur_mpp.ToString("n0"), Color.White), new Point(0, this.screen_height - 86));
                this.screen.Blit(font.Render("Cursor Position:", Color.White), new Point(0, this.screen_height - 74));
                if ((int)mouseXMeters == 0) this.screen.Blit(font.Render("x=" + (mouseXMeters).ToString(), Color.White), new Point(0, this.screen_height - 62));
                else this.screen.Blit(font.Render("x=" + (mouseXMeters).ToString("n0"), Color.White), new Point(0, this.screen_height - 62));
                if ((int)mouseYMeters == 0) this.screen.Blit(font.Render("y=" + (mouseYMeters).ToString(), Color.White), new Point(0, this.screen_height - 50));
                else this.screen.Blit(font.Render("y=" + (mouseYMeters).ToString("n0"), Color.White), new Point(0, this.screen_height - 50));
                this.screen.Blit(font.Render("Center Position:", Color.White), new Point(0, this.screen_height - 38));
                if ((int)this.cur_X == 0) this.screen.Blit(font.Render("x=" + this.cur_X.ToString(), Color.White), new Point(0, this.screen_height - 26));
                else this.screen.Blit(font.Render("x=" + this.cur_X.ToString("n0"), Color.White), new Point(0, this.screen_height - 26));
                if ((int)this.cur_Y == 0) this.screen.Blit(font.Render("y=" + this.cur_Y.ToString(), Color.White), new Point(0, this.screen_height - 14));
                else this.screen.Blit(font.Render("y=" + this.cur_Y.ToString("n0"), Color.White), new Point(0, this.screen_height - 14));
            }
            private void CaptureScreen()
            {
                if (this.capture_screen && this.time_past_since_capture.Elapsed.TotalSeconds > this.spc)
                {
                    this.screen.SaveBmp(Application.StartupPath + "\\" + (this.num_of_captured++) + ".bmp");
                    this.time_past_since_capture.Reset();
                    this.time_past_since_capture.Start();
                }
            }
            private void BlankScreen()
            {
                if (!refresh_screen)
                {
                    this.screen.Fill(new Rectangle(0, 0, this.screen_width, 12 * this.onscreen_lines.Count), Color.Black);
                    this.screen.Fill(new Rectangle(0, this.screen_height - 86, this.screen_width, 86), Color.Black);
                }
                else this.screen.Fill(Color.Black);
            }
            private void DrawBodies()
            {
                if (this.two_dimensional)
                {
                    for (int i = 0; i < this.bodies2D.Length; i++)
                    {
                        double x = this.screen_width / 2 + (this.bodies2D[i]._positionX - this.cur_X) / this.cur_mpp;
                        double y = this.screen_height / 2 - (this.bodies2D[i]._positionY - this.cur_Y) / this.cur_mpp;
                        double r = this.visual_props[i]._radius / this.cur_mpp;
                        if ((int)r == 0) r++;
                        if (x < (double)int.MinValue || x > (double)int.MaxValue ||
                            y < (double)int.MinValue || y > (double)int.MaxValue ||
                            r < (double)int.MinValue || r > (double)int.MaxValue)
                            continue;
                        if ((x + r > 0 && x + r < this.screen_width && y + r > 0 && y + r < this.screen_height) || //Left-Down corner is on the screen
                            (x + r > 0 && x + r < this.screen_width && y - r > 0 && y - r < this.screen_height) || //Left-Up corner is on the screen
                            (x - r > 0 && x - r < this.screen_width && y + r > 0 && y + r < this.screen_height) || //Right-Down corner is on the screen
                            (x - r > 0 && x - r < this.screen_width && y - r > 0 && y - r < this.screen_height) || //Right-Up corner is on the screen
                            (x - r > 0 && x + r < this.screen_width && y - r < 0 && y + r > this.screen_height) || //Vertically beyond the screen
                            (x - r < 0 && x + r > this.screen_width && y - r > 0 && y + r < this.screen_height)) //Horizontally beyond the screen
                            this.screen.Draw(new Circle((short)x, (short)y, (short)r), this.visual_props[i]._color, true, true);
                        else if (x - r < 0 && x + r > this.screen_width && y - r < 0 && y + r > this.screen_height) //Horizontally and vertically beyond the screen
                            this.screen.Fill(this.visual_props[i]._color);
                    }
                }
                else
                {
                    for (int i = 0; i < this.bodies3D.Length; i++)
                    {
                        double x = this.screen_width / 2 + (this.bodies3D[i]._positionX - this.cur_X) / this.cur_mpp;
                        double y = this.screen_height / 2 - (this.bodies3D[i]._positionY - this.cur_Y) / this.cur_mpp;
                        double r = this.visual_props[i]._radius / this.cur_mpp;
                        if ((int)r == 0) r++;
                        if (x < (double)int.MinValue || x > (double)int.MaxValue ||
                            y < (double)int.MinValue || y > (double)int.MaxValue ||
                            r < (double)int.MinValue || r > (double)int.MaxValue)
                            continue;
                        if ((x + r > 0 && x + r < this.screen_width && y + r > 0 && y + r < this.screen_height) || //Left-Down corner is on the screen
                            (x + r > 0 && x + r < this.screen_width && y - r > 0 && y - r < this.screen_height) || //Left-Up corner is on the screen
                            (x - r > 0 && x - r < this.screen_width && y + r > 0 && y + r < this.screen_height) || //Right-Down corner is on the screen
                            (x - r > 0 && x - r < this.screen_width && y - r > 0 && y - r < this.screen_height) || //Right-Up corner is on the screen
                            (x - r > 0 && x + r < this.screen_width && y - r < 0 && y + r > this.screen_height) || //Vertically beyond the screen
                            (x - r < 0 && x + r > this.screen_width && y - r > 0 && y + r < this.screen_height)) //Horizontally beyond the screen
                            this.screen.Draw(new Circle((short)x, (short)y, (short)r), this.visual_props[i]._color, true, true);
                        else if (x - r < 0 && x + r > this.screen_width && y - r < 0 && y + r > this.screen_height) //Horizontally and vertically beyond the screen
                            this.screen.Fill(this.visual_props[i]._color);
                    }
                }
            }
            private void DrawTrails()
            {
                for (int i = 0; i < this.dots.Count; i++)
                {
                    double x = this.screen_width / 2 + (dots[i]._x - this.cur_X) / this.cur_mpp;
                    double y = this.screen_height / 2 - (dots[i]._y - this.cur_Y) / this.cur_mpp;
                    double r = dots[i]._radius / this.cur_mpp;
                    if (x < (double)int.MinValue || x > (double)int.MaxValue ||
                        y < (double)int.MinValue || y > (double)int.MaxValue ||
                        r < (double)int.MinValue || r > (double)int.MaxValue)
                        continue;
                    if ((int)r == 0) { if (x < this.screen_width && x > 0 && y < this.screen_height && y > 0 && this.dots[i]._radius / this.cur_mpp != 0) screen.Draw(new Point((int)x, (int)y), this.dots[i]._color); }
                    else if ((x + r > 0 && x + r < this.screen_width && y + r > 0 && y + r < this.screen_height) ||
                        (x + r > 0 && x + r < this.screen_width && y - r > 0 && y - r < this.screen_height) ||
                        (x - r > 0 && x - r < this.screen_width && y + r > 0 && y + r < this.screen_height) ||
                        (x - r > 0 && x - r < this.screen_width && y - r > 0 && y - r < this.screen_height) ||
                        (x - r > 0 && x + r < this.screen_width && y - r < 0 && y + r > this.screen_height) ||
                        (x - r < 0 && x + r > this.screen_width && y - r > 0 && y + r < this.screen_height))
                        screen.Draw(new Circle((short)x, (short)y, (short)r), this.dots[i]._color, true, true);
                    else if (x - r < 0 && x + r > this.screen_width && y - r < 0 && y + r > this.screen_height)
                        screen.Fill(this.visual_props[i]._color);
                }
            }
        #endregion
        #region Camera Handler
            private void ChangeCamera()
            {
                double seconds_past_horizontal = (double)this.pressed_horizontal.Elapsed.TotalSeconds;
                double seconds_past_vertical = (double)this.pressed_vertical.Elapsed.TotalSeconds;
                double seconds_past_zoom = (double)this.pressed_zoom.Elapsed.TotalSeconds;
                if (this.right_pressed) this.cur_X = this.CameraHelper(this.temp_X, 0.01 * this.screen_width_meters, 3 * this.screen_width_meters, seconds_past_horizontal);
                if (this.left_pressed) this.cur_X = this.CameraHelper(this.temp_X, -0.01 * this.screen_width_meters, -3 * this.screen_width_meters, seconds_past_horizontal);
                if (this.up_pressed) this.cur_Y = this.CameraHelper(this.temp_Y, 0.01 * this.screen_height_meters, 3 * this.screen_height_meters, seconds_past_vertical);
                if (this.down_pressed) this.cur_Y = this.CameraHelper(this.temp_Y, -0.01 * this.screen_height_meters, -3 * this.screen_height_meters, seconds_past_vertical);
                if (this.plus_pressed) this.cur_mpp = this.temp_mpp / Math.Pow(Math.Pow(Math.Pow(seconds_past_zoom + 1, seconds_past_zoom + 1), seconds_past_zoom + 1), seconds_past_zoom + 1);
                if (this.minus_pressed) this.cur_mpp = this.temp_mpp * Math.Pow(Math.Pow(Math.Pow(seconds_past_zoom + 1, seconds_past_zoom + 1), seconds_past_zoom + 1), seconds_past_zoom + 1);
                if (this.cur_mpp == double.NaN) this.cur_mpp = this.original_mpp;
                if (this.cur_mpp == double.PositiveInfinity) this.cur_mpp = double.MaxValue;
                if (this.cur_mpp == double.NegativeInfinity) this.cur_mpp = double.MinValue;
                if (this.cur_mpp == 0) this.cur_mpp = double.Epsilon;
                if (this.cur_X == double.NaN) this.cur_X = this.original_X;
                if (this.cur_X == double.PositiveInfinity) this.cur_X = double.MaxValue;
                if (this.cur_X == double.NegativeInfinity) this.cur_X = double.MinValue;
                if (this.cur_Y == double.NaN) this.cur_Y = this.original_Y;
                if (this.cur_Y == double.PositiveInfinity) this.cur_Y = double.MaxValue;
                if (this.cur_Y == double.NegativeInfinity) this.cur_Y = double.MinValue;
            }
            private double CameraHelper(double x0, double v0, double a, double t)
            {
                return x0 + v0 * t + a * t * t / 2;
            }
        #endregion
        #region Input Handler
            private void KeyDown(object sender, KeyboardEventArgs e)
            {
                if (e.Key == SdlDotNet.Input.Key.RightArrow) { this.right_pressed = true; this.pressed_horizontal.Start(); this.temp_X = this.cur_X; }
                if (e.Key == SdlDotNet.Input.Key.LeftArrow) { this.left_pressed = true; this.pressed_horizontal.Start(); this.temp_X = this.cur_X; }
                if (e.Key == SdlDotNet.Input.Key.DownArrow) { this.down_pressed = true; this.pressed_vertical.Start(); this.temp_Y = this.cur_Y; }
                if (e.Key == SdlDotNet.Input.Key.UpArrow) { this.up_pressed = true; this.pressed_vertical.Start(); this.temp_Y = this.cur_Y; }
                if (e.Key == SdlDotNet.Input.Key.KeypadPlus) { this.plus_pressed = true; this.pressed_zoom.Start(); this.temp_mpp = this.cur_mpp; }
                if (e.Key == SdlDotNet.Input.Key.KeypadMinus) { this.minus_pressed = true; this.pressed_zoom.Start(); this.temp_mpp = this.cur_mpp; }
                if (e.Key == SdlDotNet.Input.Key.R) { this.cur_X = this.original_X; this.cur_Y = this.original_Y; this.cur_mpp = this.original_mpp; }
                if (e.Key == SdlDotNet.Input.Key.T) this.show_trail = !this.show_trail;
                if (e.Key == SdlDotNet.Input.Key.U) this.save_trail = !this.save_trail;
                if (e.Key == SdlDotNet.Input.Key.B) this.refresh_screen = !this.refresh_screen;
                if (e.Key == SdlDotNet.Input.Key.Q) this.dots.Clear();
                if (e.Key == SdlDotNet.Input.Key.Escape) this.QuitApplication();

                if (e.Key == SdlDotNet.Input.Key.P)
                {
                    this.pause = !this.pause;
                    if (this.pause) this.time_past.Stop();
                    else this.time_past.Start();
                }
            }
            private void KeyUp(object sender, KeyboardEventArgs e)
            {
                if (e.Key == SdlDotNet.Input.Key.RightArrow) { this.pressed_horizontal.Reset(); this.right_pressed = false; }
                if (e.Key == SdlDotNet.Input.Key.LeftArrow) { this.pressed_horizontal.Reset(); this.left_pressed = false; }
                if (e.Key == SdlDotNet.Input.Key.DownArrow) { this.pressed_vertical.Reset(); this.down_pressed = false; }
                if (e.Key == SdlDotNet.Input.Key.UpArrow) { this.pressed_vertical.Reset(); this.up_pressed = false; }
                if (e.Key == SdlDotNet.Input.Key.KeypadPlus) { this.pressed_zoom.Reset(); this.plus_pressed = false; }
                if (e.Key == SdlDotNet.Input.Key.KeypadMinus) { this.pressed_zoom.Reset(); this.minus_pressed = false; }
            }
            private void MouseMove(object sender, MouseMotionEventArgs e)
            {
                this.mouseX = e.Position.X;
                this.mouseY = e.Position.Y;
            }
        #endregion
        #region Threads
            void Calc()
            {
                while (true)
                {
                    if (this.quit_threads) break;
                    if (!this.pause) { RunNextFrameVersion(this.version, this.two_dimensional); this.done_calculations++; this.done_calculations_lastsecond++; }
                }
            }
            void CalcWait()
            {
                while (true)
                {
                    if (this.quit_threads) break;
                    if (!this.pause) { RunNextFrameVersion(this.version, this.two_dimensional); this.done_calculations++; this.done_calculations_lastsecond++; }
                    Thread.Sleep(this.timeout);
                }
            }
            void CalcSpeed()
            {
                while (true)
                {
                    if (this.quit_threads) break;
                    Thread.Sleep(1000);
                    this.cur_fps = (double)this.done_frames_lastsecond;
                    if (!this.pause) this.cur_cps = (double)this.done_calculations_lastsecond;
                    else this.cur_cps = 0;
                    this.done_frames_lastsecond = 0;
                    this.done_calculations_lastsecond = 0;
                    this.curEner = this.GetEner();
                }
            }
            void AddDots()
            {
                while (true)
                {
                    if (this.quit_threads) break;
                    if (!this.save_trail) Thread.Sleep(1000);
                    else if (this.activate_dot_thread)
                    {
                        if (!this.pause && this.save_trail)
                            for (int i = 0; i < this.bodies2D.Length; i++)
                                if (this.visual_props[i]._trail != Color.Transparent && this.visual_props[i]._trailradius != 0)
                                    if (this.two_dimensional) this.dots.Add(new TrailDot(this.bodies2D[i]._positionX, this.bodies2D[i]._positionY, this.visual_props[i]._trailradius, this.visual_props[i]._trail));
                                    else this.dots.Add(new TrailDot(this.bodies3D[i]._positionX, this.bodies3D[i]._positionY, this.visual_props[i]._trailradius, this.visual_props[i]._trail));
                        while (this.dots.Count > this.maxdots) this.dots.RemoveAt(0);
                        this.activate_dot_thread = false;
                    }
                }
            }
        #endregion

        //Calculations
        #region 2DCalculations
            public static void nextFrame2DV0(Body2D[] b, //Bodies
                double dt, //The dt
                double g //The constant G
                )
            {
                double dx, dy; //used to find the connecting vector between two bodies. The subtract vector.
                double length; //The length of the subtract vector.
                double force; //The force(newtons) of gravitation between the two bodies.

                double ax, ay; //The acceleration of the bodies.
                for (int i = 0; i < b.Length; i++)
                    for (int j = 0; j < b.Length; j++) //Run on every two bodies
                    {
                        //Generate subtract vector
                        dx = b[i]._positionX - b[j]._positionX;
                        dy = b[i]._positionY - b[j]._positionY;

                        //Generate force size
                        length = Math.Sqrt((dx * dx) + (dy * dy));
                        if (length == 0) force = 0;
                        else force = (g * b[i]._mass * b[j]._mass) / (length * length);

                        //Generate the force vector. Save it instead of the subtract vector
                        if (length == 0)
                        {
                            dx = 0;
                            dy = 0;
                        }
                        else
                        {
                            dx /= length;
                            dy /= length;
                            dx *= force;
                            dy *= force;
                        }

                        //Add the force vector to the body
                        b[j]._forceX += dx; b[j]._forceY += dy;
                    }
                for (int i = 0; i < b.Length; i++) //Calculate the new position and velocity of every body.
                {
                    //Calculate the acceleration of the body
                    ax = b[i]._forceX / b[i]._mass;
                    ay = b[i]._forceY / b[i]._mass;

                    //Calculate the new position of the body
                    b[i]._positionX += b[i]._velocityX * dt + ax * dt * dt / 2;
                    b[i]._positionY += b[i]._velocityY * dt + ay * dt * dt / 2;

                    //Calculate the new velocity of the body
                    b[i]._velocityX += ax * dt;
                    b[i]._velocityY += ay * dt;

                    //Reset Forces
                    b[i]._forceX = 0; b[i]._forceY = 0;
                }
            }
            public static void nextFrame2DV1(Body2D[] b, //Bodies
                double dt, //The dt
                double g //The constant G
                )
            {
                double dx, dy; //used to find the connecting vector between two bodies. The subtract vector.
                double length; //The length of the subtract vector.
                double force; //The force(newtons) of gravitation between the two bodies.

                double ax, ay; //The acceleration of the bodies.
                for (int i = 0; i < b.Length - 1; i++)
                    for (int j = i + 1; j < b.Length; j++) //Run on every two bodies
                    {
                        //Generate subtract vector
                        dx = b[i]._positionX - b[j]._positionX;
                        dy = b[i]._positionY - b[j]._positionY;

                        //Generate force size
                        length = Math.Sqrt((dx * dx) + (dy * dy));
                        if (length == 0) force = 0;
                        else force = (g * b[i]._mass * b[j]._mass) / (length * length);

                        //Generate the force vector. Save it instead of the subtract vector
                        if (length == 0)
                        {
                            dx = 0;
                            dy = 0;
                        }
                        else
                        {
                            dx /= length;
                            dy /= length;
                            dx *= force;
                            dy *= force;
                        }

                        //Add the force vector to the bodies
                        b[i]._forceX -= dx; b[i]._forceY -= dy;
                        b[j]._forceX += dx; b[j]._forceY += dy;
                    }
                for (int i = 0; i < b.Length; i++) //Calculate the new position and velocity of every body.
                {
                    //Calculate the acceleration of the body
                    ax = b[i]._forceX / b[i]._mass;
                    ay = b[i]._forceY / b[i]._mass;

                    //Calculate the new position of the body
                    b[i]._positionX += b[i]._velocityX * dt + ax * dt * dt / 2;
                    b[i]._positionY += b[i]._velocityY * dt + ay * dt * dt / 2;

                    //Calculate the new velocity of the body
                    b[i]._velocityX += ax * dt;
                    b[i]._velocityY += ay * dt;

                    //Reset Forces
                    b[i]._forceX = 0; b[i]._forceY = 0;
                }
            }
            public static void nextFrame2DV2(Body2D[] b, //Bodies
                double dt, //The dt
                double g //The constant G
                )
            {
                double dx, dy; //used to find the connecting vector between two bodies. The subtract vector.
                double length; //The length of the subtract vector.
                double forceDivLength; //The force(newtons) of gravitation between the two bodies, divided by the length of the subtract vector. The subtract vector is multiplied by this value to generate the force vector.

                double ax, ay; //The acceleration of the bodies.
                for (int i = 0; i < b.Length - 1; i++)
                    for (int j = i + 1; j < b.Length; j++) //Run on every two bodies
                    {
                        //Generate subtract vector
                        dx = b[i]._positionX - b[j]._positionX;
                        dy = b[i]._positionY - b[j]._positionY;

                        //Generate force size
                        length = Math.Sqrt((dx * dx) + (dy * dy));
                        if (length == 0) forceDivLength = 0;
                        else forceDivLength = (g * b[i]._mass * b[j]._mass) / (length * length * length);

                        //Generate the force vector. Save it instead of the subtract vector
                        dx *= forceDivLength;
                        dy *= forceDivLength;

                        //Add the force vector to the bodies
                        b[i]._forceX -= dx; b[i]._forceY -= dy;
                        b[j]._forceX += dx; b[j]._forceY += dy;
                    }
                for (int i = 0; i < b.Length; i++) //Calculate the new position and velocity of every body.
                {
                    //Calculate the acceleration of the body
                    ax = b[i]._forceX / b[i]._mass;
                    ay = b[i]._forceY / b[i]._mass;

                    //Calculate the new position of the body
                    b[i]._positionX += b[i]._velocityX * dt + ax * dt * dt / 2;
                    b[i]._positionY += b[i]._velocityY * dt + ay * dt * dt / 2;

                    //Calculate the new velocity of the body
                    b[i]._velocityX += ax * dt;
                    b[i]._velocityY += ay * dt;

                    //Reset Forces
                    b[i]._forceX = 0; b[i]._forceY = 0;
                }
            }
            public static void nextFrame2DV3(Body2D[] b, //Bodies
                double dt, //The dt
                double g //The constant G
                )
            {
                double dx, dy; //used to find the connecting vector between two bodies. The subtract vector.
                double length2; //The square of the length of the subtract vector.
                double forceDivLength; //The force(newtons) of gravitation between the two bodies, divided by the length of the subtract vector. The subtract vector is multiplied by this value to generate the force vector.

                double adtx, adty; //The acceleration of the bodies multiplied by dt(the acceleration alone is not used).
                for (int i = 0; i < b.Length - 1; i++)
                    for (int j = i + 1; j < b.Length; j++) //Run on every two bodies
                    {
                        //Generate subtract vector
                        dx = b[i]._positionX - b[j]._positionX;
                        dy = b[i]._positionY - b[j]._positionY;

                        //Generate force size
                        length2 = (dx * dx) + (dy * dy);
                        if (length2 == 0) forceDivLength = 0;
                        else forceDivLength = (g * b[i]._mass * b[j]._mass) / (length2 * Math.Sqrt(length2));

                        //Generate the force vector. Save it instead of the subtract vector
                        dx *= forceDivLength;
                        dy *= forceDivLength;

                        //Add the force vector to the bodies
                        b[i]._forceX -= dx; b[i]._forceY -= dy;
                        b[j]._forceX += dx; b[j]._forceY += dy;
                    }
                for (int i = 0; i < b.Length; i++) //Calculate the new position and velocity of every body.
                {
                    //Calculate the acceleration of the body
                    adtx = b[i]._forceX * dt / b[i]._mass;
                    adty = b[i]._forceY * dt / b[i]._mass;

                    //Calculate the new position of the body
                    b[i]._positionX += b[i]._velocityX * dt + adtx * dt / 2;
                    b[i]._positionY += b[i]._velocityY * dt + adty * dt / 2;

                    //Calculate the new velocity of the body
                    b[i]._velocityX += adtx;
                    b[i]._velocityY += adty;

                    //Reset Forces
                    b[i]._forceX = 0; b[i]._forceY = 0;
                }
            }
            public static void nextFrame2DV4(Body2D[] b, //Bodies
                double dt, //The dt
                double dt2, //The dt/2. Used to calculate acceleration/2*dt*dt = (acceleration*dt)*dt2
                double g //The constant G
                )
            {
                double dx, dy; //used to find the connecting vector between two bodies. The subtract vector.
                double length2; //The square of the length of the subtract vector.
                double forceDivLength; //The force(newtons) of gravitation between the two bodies, divided by the length of the subtract vector. The subtract vector is multiplied by this value to generate the force vector.

                double adtx, adty; //The acceleration of the bodies multiplied by dt(the acceleration alone is not used).
                for (int i = 0; i < b.Length - 1; i++)
                    for (int j = i + 1; j < b.Length; j++) //Run on every two bodies
                    {
                        //Generate subtract vector
                        dx = b[i]._positionX - b[j]._positionX;
                        dy = b[i]._positionY - b[j]._positionY;

                        //Generate force size
                        length2 = (dx * dx) + (dy * dy);
                        if (length2 == 0) forceDivLength = 0;
                        else forceDivLength = (g * b[i]._mass * b[j]._mass) / (length2 * Math.Sqrt(length2));

                        //Generate the force vector. Save it instead of the subtract vector
                        dx *= forceDivLength;
                        dy *= forceDivLength;

                        //Add the force vector to the bodies
                        b[i]._forceX -= dx; b[i]._forceY -= dy;
                        b[j]._forceX += dx; b[j]._forceY += dy;
                    }
                for (int i = 0; i < b.Length; i++) //Calculate the new position and velocity of every body.
                {
                    //Calculate the acceleration of the body
                    adtx = b[i]._forceX * dt / b[i]._mass;
                    adty = b[i]._forceY * dt / b[i]._mass;

                    //Calculate the new position of the body
                    b[i]._positionX += b[i]._velocityX * dt + adtx * dt2;
                    b[i]._positionY += b[i]._velocityY * dt + adty * dt2;

                    //Calculate the new velocity of the body
                    b[i]._velocityX += adtx;
                    b[i]._velocityY += adty;

                    //Reset Forces
                    b[i]._forceX = 0; b[i]._forceY = 0;
                }
            }
            //V5 is slower than V4
            public static void nextFrame2DV5(Body2D[] b, //Bodies
                int bodies, //Number of bodies
                double dt, //The dt
                double dt2, //The dt/2. Used to calculate acceleration/2*dt*dt = (acceleration*dt)*dt2
                double g //The constant G
                )
            {
                double dx, dy; //used to find the connecting vector between two bodies. The subtract vector.
                double length2; //The square of the length of the subtract vector.
                double forceDivLength; //The force(newtons) of gravitation between the two bodies, divided by the length of the subtract vector. The subtract vector is multiplied by this value to generate the force vector.

                double adtx, adty; //The acceleration of the bodies multiplied by dt(the acceleration alone is not used).
                for (int i = 0; i < bodies - 1; i++)
                    for (int j = i + 1; j < bodies; j++) //Run on every two bodies
                    {
                        //Generate subtract vector
                        dx = b[i]._positionX - b[j]._positionX;
                        dy = b[i]._positionY - b[j]._positionY;

                        //Generate force size
                        length2 = (dx * dx) + (dy * dy);
                        if (length2 == 0) forceDivLength = 0;
                        else forceDivLength = (g * b[i]._mass * b[j]._mass) / (length2 * Math.Sqrt(length2));

                        //Generate the force vector. Save it instead of the subtract vector
                        dx *= forceDivLength;
                        dy *= forceDivLength;

                        //Add the force vector to the bodies
                        b[i]._forceX -= dx; b[i]._forceY -= dy;
                        b[j]._forceX += dx; b[j]._forceY += dy;
                    }
                for (int i = 0; i < bodies; i++) //Calculate the new position and velocity of every body.
                {
                    //Calculate the acceleration of the body
                    adtx = b[i]._forceX * dt / b[i]._mass;
                    adty = b[i]._forceY * dt / b[i]._mass;

                    //Calculate the new position of the body
                    b[i]._positionX += b[i]._velocityX * dt + adtx * dt2;
                    b[i]._positionY += b[i]._velocityY * dt + adty * dt2;

                    //Calculate the new velocity of the body
                    b[i]._velocityX += adtx;
                    b[i]._velocityY += adty;

                    //Reset Forces
                    b[i]._forceX = 0; b[i]._forceY = 0;
                }
            }
            public static void nextFrame2DV6(Body2D[] b, //Bodies
                double[] massinverse, //Table of the 1/mass for every body - used to calculate acceleration
                double dt, //The dt
                double dt2, //The dt/2. Used to calculate acceleration/2*dt*dt = (acceleration*dt)*dt2
                double g //The constant G
                )
            {
                double dx, dy; //used to find the connecting vector between two bodies. The subtract vector.
                double length2; //The square of the length of the subtract vector.
                double forceDivLength; //The force(newtons) of gravitation between the two bodies, divided by the length of the subtract vector. The subtract vector is multiplied by this value to generate the force vector.

                double adtx, adty; //The acceleration of the bodies multiplied by dt(the acceleration alone is not used).
                for (int i = 0; i < b.Length - 1; i++)
                    for (int j = i + 1; j < b.Length; j++) //Run on every two bodies
                    {
                        //Generate subtract vector
                        dx = b[i]._positionX - b[j]._positionX;
                        dy = b[i]._positionY - b[j]._positionY;

                        //Generate force size
                        length2 = (dx * dx) + (dy * dy);
                        if (length2 == 0) forceDivLength = 0;
                        else forceDivLength = (g * b[i]._mass * b[j]._mass) / (length2 * Math.Sqrt(length2));

                        //Generate the force vector. Save it instead of the subtract vector
                        dx *= forceDivLength;
                        dy *= forceDivLength;

                        //Add the force vector to the bodies
                        b[i]._forceX -= dx; b[i]._forceY -= dy;
                        b[j]._forceX += dx; b[j]._forceY += dy;
                    }
                for (int i = 0; i < b.Length; i++) //Calculate the new position and velocity of every body.
                {
                    //Calculate the acceleration of the body multiplied by dt
                    adtx = b[i]._forceX * massinverse[i] * dt;
                    adty = b[i]._forceY * massinverse[i] * dt;

                    //Calculate the new position of the body
                    b[i]._positionX += b[i]._velocityX * dt + adtx * dt2;
                    b[i]._positionY += b[i]._velocityY * dt + adty * dt2;

                    //Calculate the new velocity of the body
                    b[i]._velocityX += adtx;
                    b[i]._velocityY += adty;

                    //Reset Forces
                    b[i]._forceX = 0; b[i]._forceY = 0;
                }
            }
            public static void nextFrame2DV7(Body2D[] b, //Bodies
                double[] dtmass, //Table of the dt/mass for every body - used to calculate acceleration*dt(acceleration alone is not being used)
                double dt, //The dt
                double dt2, //The dt/2. Used to calculate acceleration/2*dt*dt = (acceleration*dt)*dt2
                double g //The constant G
                )
            {
                double dx, dy; //used to find the connecting vector between two bodies. The subtract vector.
                double length2; //The square of the length of the subtract vector.
                double forceDivLength; //The force(newtons) of gravitation between the two bodies, divided by the length of the subtract vector. The subtract vector is multiplied by this value to generate the force vector.

                double adtx, adty; //The acceleration of the bodies multiplied by dt(the acceleration alone is not used).
                for (int i = 0; i < b.Length - 1; i++)
                    for (int j = i + 1; j < b.Length; j++) //Run on every two bodies
                    {
                        //Generate subtract vector
                        dx = b[i]._positionX - b[j]._positionX;
                        dy = b[i]._positionY - b[j]._positionY;

                        //Generate force size
                        length2 = (dx * dx) + (dy * dy);
                        if (length2 == 0) forceDivLength = 0;
                        else forceDivLength = (g * b[i]._mass * b[j]._mass) / (length2 * Math.Sqrt(length2));

                        //Generate the force vector. Save it instead of the subtract vector
                        dx *= forceDivLength;
                        dy *= forceDivLength;

                        //Add the force vector to the bodies
                        b[i]._forceX -= dx; b[i]._forceY -= dy;
                        b[j]._forceX += dx; b[j]._forceY += dy;
                    }
                for (int i = 0; i < b.Length; i++) //Calculate the new position and velocity of every body.
                {
                    //Calculate the acceleration of the body multiplied by dt
                    adtx = b[i]._forceX * dtmass[i];
                    adty = b[i]._forceY * dtmass[i];

                    //Calculate the new position of the body
                    b[i]._positionX += b[i]._velocityX * dt + adtx * dt2;
                    b[i]._positionY += b[i]._velocityY * dt + adty * dt2;

                    //Calculate the new velocity of the body
                    b[i]._velocityX += adtx;
                    b[i]._velocityY += adty;

                    //Reset Forces
                    b[i]._forceX = 0; b[i]._forceY = 0;
                }
            }
            public static void nextFrame2DV8(Body2D[] b, //Bodies
                double[] dtmass, //Table of the dt/mass for every body - used to calculate acceleration*dt(acceleration alone is not being used)
                double[,] massg, //Table of the mass1*mass2*g for every two bodies
                double dt, //The dt
                double dt2 //The dt/2. Used to calculate acceleration/2*dt*dt = (acceleration*dt)*dt2
                )
            {
                double dx, dy; //used to find the connecting vector between two bodies. The subtract vector.
                double length2; //The square of the length of the subtract vector.
                double forceDivLength; //The force(newtons) of gravitation between the two bodies, divided by the length of the subtract vector. The subtract vector is multiplied by this value to generate the force vector.

                double adtx, adty; //The acceleration of the bodies multiplied by dt(the acceleration alone is not used).
                for (int i = 0; i < b.Length - 1; i++)
                    for (int j = i + 1; j < b.Length; j++) //Run on every two bodies
                    {
                        //Generate subtract vector
                        dx = b[i]._positionX - b[j]._positionX;
                        dy = b[i]._positionY - b[j]._positionY;

                        //Generate force size
                        length2 = (dx * dx) + (dy * dy);
                        if (length2 == 0) forceDivLength = 0;
                        else forceDivLength = massg[i, j] / (length2 * Math.Sqrt(length2));

                        //Generate the force vector. Save it instead of the subtract vector
                        dx *= forceDivLength;
                        dy *= forceDivLength;

                        //Add the force vector to the bodies
                        b[i]._forceX -= dx; b[i]._forceY -= dy;
                        b[j]._forceX += dx; b[j]._forceY += dy;
                    }
                for (int i = 0; i < b.Length; i++) //Calculate the new position and velocity of every body.
                {
                    //Calculate the acceleration of the body multiplied by dt
                    adtx = b[i]._forceX * dtmass[i];
                    adty = b[i]._forceY * dtmass[i];

                    //Calculate the new position of the body
                    b[i]._positionX += b[i]._velocityX * dt + adtx * dt2;
                    b[i]._positionY += b[i]._velocityY * dt + adty * dt2;

                    //Calculate the new velocity of the body
                    b[i]._velocityX += adtx;
                    b[i]._velocityY += adty;

                    //Reset Forces
                    b[i]._forceX = 0; b[i]._forceY = 0;
                }
            }
            public static void nextFrame2DV9(Body2D[] b, //Bodies
                double[] dtmass, //Table of the dt/mass for every body - used to calculate acceleration*dt(acceleration alone is not being used)
                double dt, //The dt
                double dt2, //The dt/2. Used to calculate acceleration/2*dt*dt = (acceleration*dt)*dt2
                double g //The constant G
                )
            {
                double dx, dy; //used to find the connecting vector between two bodies. The subtract vector.
                double length2; //The square of the length of the subtract vector.
                double forceDivLength; //The force(newtons) of gravitation between the two bodies, divided by the length of the subtract vector. The subtract vector is multiplied by this value to generate the force vector.

                double adtx, adty; //The acceleration of the bodies multiplied by the dt(acceleration alone is not being used).
                for (int i = 1; i < b.Length - 1; i++)
                    for (int j = i + 1; j < b.Length; j++) //Run on every two bodies except for the first one
                    {
                        #region Calculate Force
                        {
                            //Generate subtract vector
                            dx = b[i]._positionX - b[j]._positionX;
                            dy = b[i]._positionY - b[j]._positionY;

                            //Generate force size
                            length2 = (dx * dx) + (dy * dy);
                            if (length2 == 0) forceDivLength = 0;
                            else forceDivLength = g * b[i]._mass * b[j]._mass / (length2 * Math.Sqrt(length2));

                            //Generate the force vector. Save it instead of the subtract vector
                            dx *= forceDivLength;
                            dy *= forceDivLength;

                            //Add the force vector to the bodies
                            b[i]._forceX -= dx; b[i]._forceY -= dy;
                            b[j]._forceX += dx; b[j]._forceY += dy;
                        }
                        #endregion
                    }
                for (int i = 1; i < b.Length; i++) //Finish the force generating by running on every body with the first one, and then calculating the new position and velocity of that body.
                {
                    #region Calculate Force
                    {
                        //Generate subtract vector between the body and the first body
                        dx = b[i]._positionX - b[0]._positionX;
                        dy = b[i]._positionY - b[0]._positionY;

                        //Generate force size
                        length2 = (dx * dx) + (dy * dy);
                        if (length2 == 0) forceDivLength = 0;
                        else forceDivLength = g * b[i]._mass * b[0]._mass / (length2 * Math.Sqrt(length2));

                        //Generate the force vector. Save it instead of the subtract vector
                        dx *= forceDivLength;
                        dy *= forceDivLength;

                        //Add the force vector to the bodies
                        b[i]._forceX -= dx; b[i]._forceY -= dy;
                        b[0]._forceX += dx; b[0]._forceY += dy;
                    }
                    #endregion
                    #region Calculate New Velocity and Position
                    {
                        //Calculate the acceleration of the body multiplied by dt
                        adtx = b[i]._forceX * dtmass[i];
                        adty = b[i]._forceY * dtmass[i];

                        //Calculate the new position of the body
                        b[i]._positionX += b[i]._velocityX * dt + adtx * dt2;
                        b[i]._positionY += b[i]._velocityY * dt + adty * dt2;

                        //Calculate the new velocity of the body
                        b[i]._velocityX += adtx;
                        b[i]._velocityY += adty;

                        //Reset Forces
                        b[i]._forceX = 0; b[i]._forceY = 0;
                    }
                    #endregion
                }
                #region Calculate New Velocity and Position
                {
                    //Calculate the acceleration of the first body multiplied by dt
                    adtx = b[0]._forceX * dtmass[0];
                    adty = b[0]._forceY * dtmass[0];

                    //Calculate the new position of the first body
                    b[0]._positionX += b[0]._velocityX * dt + adtx * dt2;
                    b[0]._positionY += b[0]._velocityY * dt + adty * dt2;

                    //Calculate the new velocity of the first body
                    b[0]._velocityX += adtx;
                    b[0]._velocityY += adty;

                    //Reset Forces
                    b[0]._forceX = 0; b[0]._forceY = 0;
                }
                #endregion
            }
            //V9 is slower than V8
            public static void nextFrame2DV10(Body2D[] b, //Bodies
                double[] dtmass, //Table of the dt/mass for every body - used to calculate acceleration*dt(acceleration alone is not being used)
                double[] massg, //Table of the g*mass for every body - used to calculate g*m1*m2
                double dt, //The dt
                double dt2 //The dt/2. Used to calculate acceleration/2*dt*dt = (acceleration*dt)*dt2
                )
            {
                double dx, dy; //used to find the connecting vector between two bodies. The subtract vector.
                double length2; //The square of the length of the subtract vector.
                double forceDivLength; //The force(newtons) of gravitation between the two bodies, divided by the length of the subtract vector. The subtract vector is multiplied by this value to generate the force vector.

                double adtx, adty; //The acceleration of the bodies multiplied by the dt(acceleration alone is not being used).
                for (int i = 1; i < b.Length - 1; i++)
                    for (int j = i + 1; j < b.Length; j++) //Run on every two bodies except for the first one
                    {
                        #region Calculate Force
                        {
                            //Generate subtract vector
                            dx = b[i]._positionX - b[j]._positionX;
                            dy = b[i]._positionY - b[j]._positionY;

                            //Generate force size
                            length2 = (dx * dx) + (dy * dy);
                            if (length2 == 0) forceDivLength = 0;
                            else forceDivLength = massg[i] * b[j]._mass / (length2 * Math.Sqrt(length2));

                            //Generate the force vector. Save it instead of the subtract vector
                            dx *= forceDivLength;
                            dy *= forceDivLength;

                            //Add the force vector to the bodies
                            b[i]._forceX -= dx; b[i]._forceY -= dy;
                            b[j]._forceX += dx; b[j]._forceY += dy;
                        }
                        #endregion
                    }
                for (int i = 1; i < b.Length; i++) //Finish the force generating by running on every body with the first one, and then calculating the new position and velocity of that body.
                {
                    #region Calculate Force
                    {
                        //Generate subtract vector between the body and the first body
                        dx = b[i]._positionX - b[0]._positionX;
                        dy = b[i]._positionY - b[0]._positionY;

                        //Generate force size
                        length2 = (dx * dx) + (dy * dy);
                        if (length2 == 0) forceDivLength = 0;
                        else forceDivLength = massg[i] * b[0]._mass / (length2 * Math.Sqrt(length2));

                        //Generate the force vector. Save it instead of the subtract vector
                        dx *= forceDivLength;
                        dy *= forceDivLength;

                        //Add the force vector to the bodies
                        b[i]._forceX -= dx; b[i]._forceY -= dy;
                        b[0]._forceX += dx; b[0]._forceY += dy;
                    }
                    #endregion
                    #region Calculate New Velocity and Position
                    {
                        //Calculate the acceleration of the body multiplied by dt
                        adtx = b[i]._forceX * dtmass[i];
                        adty = b[i]._forceY * dtmass[i];

                        //Calculate the new position of the body
                        b[i]._positionX += b[i]._velocityX * dt + adtx * dt2;
                        b[i]._positionY += b[i]._velocityY * dt + adty * dt2;

                        //Calculate the new velocity of the body
                        b[i]._velocityX += adtx;
                        b[i]._velocityY += adty;

                        //Reset Forces
                        b[i]._forceX = 0; b[i]._forceY = 0;
                    }
                    #endregion
                }
                #region Calculate New Velocity and Position
                {
                    //Calculate the acceleration of the first body multiplied by dt
                    adtx = b[0]._forceX * dtmass[0];
                    adty = b[0]._forceY * dtmass[0];

                    //Calculate the new position of the first body
                    b[0]._positionX += b[0]._velocityX * dt + adtx * dt2;
                    b[0]._positionY += b[0]._velocityY * dt + adty * dt2;

                    //Calculate the new velocity of the first body
                    b[0]._velocityX += adtx;
                    b[0]._velocityY += adty;

                    //Reset Forces
                    b[0]._forceX = 0; b[0]._forceY = 0;
                }
                #endregion
            }
        #endregion
        #region 3DCalculations
            public static void nextFrame3DV0(Body3D[] b, //Bodies
                double dt, //The dt
                double g //The constant G
                )
            {
                double dx, dy, dz; //used to find the connecting vector between two bodies. The subtract vector.
                double length; //The length of the subtract vector.
                double force; //The force(newtons) of gravitation between the two bodies.

                double ax, ay, az; //The acceleration of the bodies.
                for (int i = 0; i < b.Length; i++)
                    for (int j = 0; j < b.Length; j++) //Run on every two bodies
                    {
                        //Generate subtract vector
                        dx = b[i]._positionX - b[j]._positionX;
                        dy = b[i]._positionY - b[j]._positionY;
                        dz = b[i]._positionZ - b[j]._positionZ;

                        //Generate force size
                        length = Math.Sqrt((dx * dx) + (dy * dy) + (dz * dz));
                        if (length == 0) force = 0;
                        else force = (g * b[i]._mass * b[j]._mass) / (length * length);

                        //Generate the force vector. Save it instead of the subtract vector
                        if (length == 0)
                        {
                            dx = 0;
                            dy = 0;
                            dz = 0;
                        }
                        else
                        {
                            dx /= length;
                            dy /= length;
                            dz /= length;
                            dx *= force;
                            dy *= force;
                            dz *= force;
                        }

                        //Add the force vector to the body
                        b[j]._forceX += dx; b[j]._forceY += dy; b[j]._forceZ += dz;
                    }
                for (int i = 0; i < b.Length; i++) //Calculate the new position and velocity of every body.
                {
                    //Calculate the acceleration of the body
                    ax = b[i]._forceX / b[i]._mass;
                    ay = b[i]._forceY / b[i]._mass;
                    az = b[i]._forceZ / b[i]._mass;

                    //Calculate the new position of the body
                    b[i]._positionX += b[i]._velocityX * dt + ax * dt * dt / 2;
                    b[i]._positionY += b[i]._velocityY * dt + ay * dt * dt / 2;
                    b[i]._positionZ += b[i]._velocityZ * dt + az * dt * dt / 2;

                    //Calculate the new velocity of the body
                    b[i]._velocityX += ax * dt;
                    b[i]._velocityY += ay * dt;
                    b[i]._velocityZ += az * dt;

                    //Reset Forces
                    b[i]._forceX = 0; b[i]._forceY = 0; b[i]._forceZ = 0;
                }
            }
            public static void nextFrame3DV1(Body3D[] b, //Bodies
                double dt, //The dt
                double g //The constant G
                )
            {
                double dx, dy, dz; //used to find the connecting vector between two bodies. The subtract vector.
                double length; //The length of the subtract vector.
                double force; //The force(newtons) of gravitation between the two bodies.

                double ax, ay, az; //The acceleration of the bodies.
                for (int i = 0; i < b.Length - 1; i++)
                    for (int j = i + 1; j < b.Length; j++) //Run on every two bodies
                    {
                        //Generate subtract vector
                        dx = b[i]._positionX - b[j]._positionX;
                        dy = b[i]._positionY - b[j]._positionY;
                        dz = b[i]._positionZ - b[j]._positionZ;

                        //Generate force size
                        length = Math.Sqrt((dx * dx) + (dy * dy) + (dz * dz));
                        if (length == 0) force = 0;
                        else force = (g * b[i]._mass * b[j]._mass) / (length * length);

                        //Generate the force vector. Save it instead of the subtract vector
                        if (length == 0)
                        {
                            dx = 0;
                            dy = 0;
                            dz = 0;
                        }
                        else
                        {
                            dx /= length;
                            dy /= length;
                            dz /= length;
                            dx *= force;
                            dy *= force;
                            dz *= force;
                        }

                        //Add the force vector to the bodies
                        b[i]._forceX -= dx; b[i]._forceY -= dy; b[i]._forceZ -= dz;
                        b[j]._forceX += dx; b[j]._forceY += dy; b[j]._forceZ += dz;
                    }
                for (int i = 0; i < b.Length; i++) //Calculate the new position and velocity of every body.
                {
                    //Calculate the acceleration of the body
                    ax = b[i]._forceX / b[i]._mass;
                    ay = b[i]._forceY / b[i]._mass;
                    az = b[i]._forceZ / b[i]._mass;

                    //Calculate the new position of the body
                    b[i]._positionX += b[i]._velocityX * dt + ax * dt * dt / 2;
                    b[i]._positionY += b[i]._velocityY * dt + ay * dt * dt / 2;
                    b[i]._positionZ += b[i]._velocityZ * dt + az * dt * dt / 2;

                    //Calculate the new velocity of the body
                    b[i]._velocityX += ax * dt;
                    b[i]._velocityY += ay * dt;
                    b[i]._velocityZ += az * dt;

                    //Reset Forces
                    b[i]._forceX = 0; b[i]._forceY = 0; b[i]._forceZ = 0;
                }
            }
            public static void nextFrame3DV2(Body3D[] b, //Bodies
                double dt, //The dt
                double g //The constant G
                )
            {
                double dx, dy, dz; //used to find the connecting vector between two bodies. The subtract vector.
                double length; //The length of the subtract vector.
                double forceDivLength; //The force(newtons) of gravitation between the two bodies, divided by the length of the subtract vector. The subtract vector is multiplied by this value to generate the force vector.

                double ax, ay, az; //The acceleration of the bodies.
                for (int i = 0; i < b.Length - 1; i++)
                    for (int j = i + 1; j < b.Length; j++) //Run on every two bodies
                    {
                        //Generate subtract vector
                        dx = b[i]._positionX - b[j]._positionX;
                        dy = b[i]._positionY - b[j]._positionY;
                        dz = b[i]._positionZ - b[j]._positionZ;

                        //Generate force size
                        length = Math.Sqrt((dx * dx) + (dy * dy) + (dz * dz));
                        if (length == 0) forceDivLength = 0;
                        else forceDivLength = (g * b[i]._mass * b[j]._mass) / (length * length * length);

                        //Generate the force vector. Save it instead of the subtract vector
                        dx *= forceDivLength;
                        dy *= forceDivLength;
                        dz *= forceDivLength;

                        //Add the force vector to the bodies
                        b[i]._forceX -= dx; b[i]._forceY -= dy; b[i]._forceZ -= dz;
                        b[j]._forceX += dx; b[j]._forceY += dy; b[j]._forceZ += dz;
                    }
                for (int i = 0; i < b.Length; i++) //Calculate the new position and velocity of every body.
                {
                    //Calculate the acceleration of the body
                    ax = b[i]._forceX / b[i]._mass;
                    ay = b[i]._forceY / b[i]._mass;
                    az = b[i]._forceZ / b[i]._mass;

                    //Calculate the new position of the body
                    b[i]._positionX += b[i]._velocityX * dt + ax * dt * dt / 2;
                    b[i]._positionY += b[i]._velocityY * dt + ay * dt * dt / 2;
                    b[i]._positionZ += b[i]._velocityZ * dt + az * dt * dt / 2;

                    //Calculate the new velocity of the body
                    b[i]._velocityX += ax * dt;
                    b[i]._velocityY += ay * dt;
                    b[i]._velocityZ += az * dt;

                    //Reset Forces
                    b[i]._forceX = 0; b[i]._forceY = 0; b[i]._forceZ = 0;
                }
            }
            public static void nextFrame3DV3(Body3D[] b, //Bodies
                double dt, //The dt
                double g //The constant G
                )
            {
                double dx, dy, dz; //used to find the connecting vector between two bodies. The subtract vector.
                double length2; //The square of the length of the subtract vector.
                double forceDivLength; //The force(newtons) of gravitation between the two bodies, divided by the length of the subtract vector. The subtract vector is multiplied by this value to generate the force vector.

                double adtx, adty, adtz; //The acceleration of the bodies multiplied by dt(the acceleration alone is not used).
                for (int i = 0; i < b.Length - 1; i++)
                    for (int j = i + 1; j < b.Length; j++) //Run on every two bodies
                    {
                        //Generate subtract vector
                        dx = b[i]._positionX - b[j]._positionX;
                        dy = b[i]._positionY - b[j]._positionY;
                        dz = b[i]._positionZ - b[j]._positionZ;

                        //Generate force size
                        length2 = (dx * dx) + (dy * dy) + (dz * dz);
                        if (length2 == 0) forceDivLength = 0;
                        else forceDivLength = (g * b[i]._mass * b[j]._mass) / (length2 * Math.Sqrt(length2));

                        //Generate the force vector. Save it instead of the subtract vector
                        dx *= forceDivLength;
                        dy *= forceDivLength;
                        dz *= forceDivLength;

                        //Add the force vector to the bodies
                        b[i]._forceX -= dx; b[i]._forceY -= dy; b[i]._forceZ -= dz;
                        b[j]._forceX += dx; b[j]._forceY += dy; b[j]._forceZ += dz;
                    }
                for (int i = 0; i < b.Length; i++) //Calculate the new position and velocity of every body.
                {
                    //Calculate the acceleration of the body
                    adtx = b[i]._forceX * dt / b[i]._mass;
                    adty = b[i]._forceY * dt / b[i]._mass;
                    adtz = b[i]._forceZ * dt / b[i]._mass;

                    //Calculate the new position of the body
                    b[i]._positionX += b[i]._velocityX * dt + adtx * dt / 2;
                    b[i]._positionY += b[i]._velocityY * dt + adty * dt / 2;
                    b[i]._positionZ += b[i]._velocityZ * dt + adtz * dt / 2;

                    //Calculate the new velocity of the body
                    b[i]._velocityX += adtx;
                    b[i]._velocityY += adty;
                    b[i]._velocityZ += adtz;

                    //Reset Forces
                    b[i]._forceX = 0; b[i]._forceY = 0; b[i]._forceZ = 0;
                }
            }
            public static void nextFrame3DV4(Body3D[] b, //Bodies
                double dt, //The dt
                double dt2, //The dt/2. Used to calculate acceleration/2*dt*dt = (acceleration*dt)*dt2
                double g //The constant G
                )
            {
                double dx, dy, dz; //used to find the connecting vector between two bodies. The subtract vector.
                double length2; //The square of the length of the subtract vector.
                double forceDivLength; //The force(newtons) of gravitation between the two bodies, divided by the length of the subtract vector. The subtract vector is multiplied by this value to generate the force vector.

                double adtx, adty, adtz; //The acceleration of the bodies multiplied by dt(the acceleration alone is not used).
                for (int i = 0; i < b.Length - 1; i++)
                    for (int j = i + 1; j < b.Length; j++) //Run on every two bodies
                    {
                        //Generate subtract vector
                        dx = b[i]._positionX - b[j]._positionX;
                        dy = b[i]._positionY - b[j]._positionY;
                        dz = b[i]._positionZ - b[j]._positionZ;

                        //Generate force size
                        length2 = (dx * dx) + (dy * dy) + (dz * dz);
                        if (length2 == 0) forceDivLength = 0;
                        else forceDivLength = (g * b[i]._mass * b[j]._mass) / (length2 * Math.Sqrt(length2));

                        //Generate the force vector. Save it instead of the subtract vector
                        dx *= forceDivLength;
                        dy *= forceDivLength;
                        dz *= forceDivLength;

                        //Add the force vector to the bodies
                        b[i]._forceX -= dx; b[i]._forceY -= dy; b[i]._forceZ -= dz;
                        b[j]._forceX += dx; b[j]._forceY += dy; b[j]._forceZ += dz;
                    }
                for (int i = 0; i < b.Length; i++) //Calculate the new position and velocity of every body.
                {
                    //Calculate the acceleration of the body
                    adtx = b[i]._forceX * dt / b[i]._mass;
                    adty = b[i]._forceY * dt / b[i]._mass;
                    adtz = b[i]._forceZ * dt / b[i]._mass;

                    //Calculate the new position of the body
                    b[i]._positionX += b[i]._velocityX * dt + adtx * dt2;
                    b[i]._positionY += b[i]._velocityY * dt + adty * dt2;
                    b[i]._positionZ += b[i]._velocityZ * dt + adtz * dt2;

                    //Calculate the new velocity of the body
                    b[i]._velocityX += adtx;
                    b[i]._velocityY += adty;
                    b[i]._velocityZ += adtz;

                    //Reset Forces
                    b[i]._forceX = 0; b[i]._forceY = 0; b[i]._forceZ = 0;
                }
            }
            //V5 is slower than V4
            public static void nextFrame3DV5(Body3D[] b, //Bodies
                int bodies, //Number of bodies
                double dt, //The dt
                double dt2, //The dt/2. Used to calculate acceleration/2*dt*dt = (acceleration*dt)*dt2
                double g //The constant G
                )
            {
                double dx, dy, dz; //used to find the connecting vector between two bodies. The subtract vector.
                double length2; //The square of the length of the subtract vector.
                double forceDivLength; //The force(newtons) of gravitation between the two bodies, divided by the length of the subtract vector. The subtract vector is multiplied by this value to generate the force vector.

                double adtx, adty, adtz; //The acceleration of the bodies multiplied by dt(the acceleration alone is not used).
                for (int i = 0; i < bodies - 1; i++)
                    for (int j = i + 1; j < bodies; j++) //Run on every two bodies
                    {
                        //Generate subtract vector
                        dx = b[i]._positionX - b[j]._positionX;
                        dy = b[i]._positionY - b[j]._positionY;
                        dz = b[i]._positionZ - b[j]._positionZ;

                        //Generate force size
                        length2 = (dx * dx) + (dy * dy) + (dz * dz);
                        if (length2 == 0) forceDivLength = 0;
                        else forceDivLength = (g * b[i]._mass * b[j]._mass) / (length2 * Math.Sqrt(length2));

                        //Generate the force vector. Save it instead of the subtract vector
                        dx *= forceDivLength;
                        dy *= forceDivLength;
                        dz *= forceDivLength;

                        //Add the force vector to the bodies
                        b[i]._forceX -= dx; b[i]._forceY -= dy; b[i]._forceZ -= dz;
                        b[j]._forceX += dx; b[j]._forceY += dy; b[j]._forceZ += dz;
                    }
                for (int i = 0; i < bodies; i++) //Calculate the new position and velocity of every body.
                {
                    //Calculate the acceleration of the body
                    adtx = b[i]._forceX * dt / b[i]._mass;
                    adty = b[i]._forceY * dt / b[i]._mass;
                    adtz = b[i]._forceZ * dt / b[i]._mass;

                    //Calculate the new position of the body
                    b[i]._positionX += b[i]._velocityX * dt + adtx * dt2;
                    b[i]._positionY += b[i]._velocityY * dt + adty * dt2;
                    b[i]._positionZ += b[i]._velocityZ * dt + adtz * dt2;

                    //Calculate the new velocity of the body
                    b[i]._velocityX += adtx;
                    b[i]._velocityY += adty;
                    b[i]._velocityZ += adtz;

                    //Reset Forces
                    b[i]._forceX = 0; b[i]._forceY = 0; b[i]._forceZ = 0;
                }
            }
            public static void nextFrame3DV6(Body3D[] b, //Bodies
                double[] massinverse, //Table of the 1/mass for every body - used to calculate acceleration
                double dt, //The dt
                double dt2, //The dt/2. Used to calculate acceleration/2*dt*dt = (acceleration*dt)*dt2
                double g //The constant G
                )
            {
                double dx, dy, dz; //used to find the connecting vector between two bodies. The subtract vector.
                double length2; //The square of the length of the subtract vector.
                double forceDivLength; //The force(newtons) of gravitation between the two bodies, divided by the length of the subtract vector. The subtract vector is multiplied by this value to generate the force vector.

                double adtx, adty, adtz; //The acceleration of the bodies multiplied by dt(the acceleration alone is not used).
                for (int i = 0; i < b.Length - 1; i++)
                    for (int j = i + 1; j < b.Length; j++) //Run on every two bodies
                    {
                        //Generate subtract vector
                        dx = b[i]._positionX - b[j]._positionX;
                        dy = b[i]._positionY - b[j]._positionY;
                        dz = b[i]._positionZ - b[j]._positionZ;

                        //Generate force size
                        length2 = (dx * dx) + (dy * dy) + (dz * dz);
                        if (length2 == 0) forceDivLength = 0;
                        else forceDivLength = (g * b[i]._mass * b[j]._mass) / (length2 * Math.Sqrt(length2));

                        //Generate the force vector. Save it instead of the subtract vector
                        dx *= forceDivLength;
                        dy *= forceDivLength;
                        dz *= forceDivLength;

                        //Add the force vector to the bodies
                        b[i]._forceX -= dx; b[i]._forceY -= dy; b[i]._forceZ -= dz;
                        b[j]._forceX += dx; b[j]._forceY += dy; b[j]._forceZ += dz;
                    }
                for (int i = 0; i < b.Length; i++) //Calculate the new position and velocity of every body.
                {
                    //Calculate the acceleration of the body multiplied by dt
                    adtx = b[i]._forceX * massinverse[i] * dt;
                    adty = b[i]._forceY * massinverse[i] * dt;
                    adtz = b[i]._forceZ * massinverse[i] * dt;

                    //Calculate the new position of the body
                    b[i]._positionX += b[i]._velocityX * dt + adtx * dt2;
                    b[i]._positionY += b[i]._velocityY * dt + adty * dt2;
                    b[i]._positionZ += b[i]._velocityZ * dt + adtz * dt2;

                    //Calculate the new velocity of the body
                    b[i]._velocityX += adtx;
                    b[i]._velocityY += adty;
                    b[i]._velocityZ += adtz;

                    //Reset Forces
                    b[i]._forceX = 0; b[i]._forceY = 0; b[i]._forceZ = 0;
                }
            }
            public static void nextFrame3DV7(Body3D[] b, //Bodies
                double[] dtmass, //Table of the dt/mass for every body - used to calculate acceleration*dt(acceleration alone is not being used)
                double dt, //The dt
                double dt2, //The dt/2. Used to calculate acceleration/2*dt*dt = (acceleration*dt)*dt2
                double g //The constant G
                )
            {
                double dx, dy, dz; //used to find the connecting vector between two bodies. The subtract vector.
                double length2; //The square of the length of the subtract vector.
                double forceDivLength; //The force(newtons) of gravitation between the two bodies, divided by the length of the subtract vector. The subtract vector is multiplied by this value to generate the force vector.

                double adtx, adty, adtz; //The acceleration of the bodies multiplied by dt(the acceleration alone is not used).
                for (int i = 0; i < b.Length - 1; i++)
                    for (int j = i + 1; j < b.Length; j++) //Run on every two bodies
                    {
                        //Generate subtract vector
                        dx = b[i]._positionX - b[j]._positionX;
                        dy = b[i]._positionY - b[j]._positionY;
                        dz = b[i]._positionZ - b[j]._positionZ;

                        //Generate force size
                        length2 = (dx * dx) + (dy * dy) + (dz * dz);
                        if (length2 == 0) forceDivLength = 0;
                        else forceDivLength = (g * b[i]._mass * b[j]._mass) / (length2 * Math.Sqrt(length2));

                        //Generate the force vector. Save it instead of the subtract vector
                        dx *= forceDivLength;
                        dy *= forceDivLength;
                        dz *= forceDivLength;

                        //Add the force vector to the bodies
                        b[i]._forceX -= dx; b[i]._forceY -= dy; b[i]._forceZ -= dz;
                        b[j]._forceX += dx; b[j]._forceY += dy; b[j]._forceZ += dz;
                    }
                for (int i = 0; i < b.Length; i++) //Calculate the new position and velocity of every body.
                {
                    //Calculate the acceleration of the body multiplied by dt
                    adtx = b[i]._forceX * dtmass[i];
                    adty = b[i]._forceY * dtmass[i];
                    adtz = b[i]._forceY * dtmass[i];

                    //Calculate the new position of the body
                    b[i]._positionX += b[i]._velocityX * dt + adtx * dt2;
                    b[i]._positionY += b[i]._velocityY * dt + adty * dt2;
                    b[i]._positionZ += b[i]._velocityZ * dt + adtz * dt2;

                    //Calculate the new velocity of the body
                    b[i]._velocityX += adtx;
                    b[i]._velocityY += adty;
                    b[i]._velocityZ += adtz;

                    //Reset Forces
                    b[i]._forceX = 0; b[i]._forceY = 0; b[i]._forceZ = 0;
                }
            }
            public static void nextFrame3DV8(Body3D[] b, //Bodies
                double[] dtmass, //Table of the dt/mass for every body - used to calculate acceleration*dt(acceleration alone is not being used)
                double[,] massg, //Table of the mass1*mass2*g for every two bodies
                double dt, //The dt
                double dt2 //The dt/2. Used to calculate acceleration/2*dt*dt = (acceleration*dt)*dt2
                )
            {
                double dx, dy, dz; //used to find the connecting vector between two bodies. The subtract vector.
                double length2; //The square of the length of the subtract vector.
                double forceDivLength; //The force(newtons) of gravitation between the two bodies, divided by the length of the subtract vector. The subtract vector is multiplied by this value to generate the force vector.

                double adtx, adty, adtz; //The acceleration of the bodies multiplied by dt(the acceleration alone is not used).
                for (int i = 0; i < b.Length - 1; i++)
                    for (int j = i + 1; j < b.Length; j++) //Run on every two bodies
                    {
                        //Generate subtract vector
                        dx = b[i]._positionX - b[j]._positionX;
                        dy = b[i]._positionY - b[j]._positionY;
                        dz = b[i]._positionZ - b[j]._positionZ;

                        //Generate force size
                        length2 = (dx * dx) + (dy * dy) + (dz * dz);
                        if (length2 == 0) forceDivLength = 0;
                        else forceDivLength = massg[i, j] / (length2 * Math.Sqrt(length2));

                        //Generate the force vector. Save it instead of the subtract vector
                        dx *= forceDivLength;
                        dy *= forceDivLength;
                        dz *= forceDivLength;

                        //Add the force vector to the bodies
                        b[i]._forceX -= dx; b[i]._forceY -= dy; b[i]._forceZ -= dz;
                        b[j]._forceX += dx; b[j]._forceY += dy; b[j]._forceZ += dz;
                    }
                for (int i = 0; i < b.Length; i++) //Calculate the new position and velocity of every body.
                {
                    //Calculate the acceleration of the body multiplied by dt
                    adtx = b[i]._forceX * dtmass[i];
                    adty = b[i]._forceY * dtmass[i];
                    adtz = b[i]._forceZ * dtmass[i];

                    //Calculate the new position of the body
                    b[i]._positionX += b[i]._velocityX * dt + adtx * dt2;
                    b[i]._positionY += b[i]._velocityY * dt + adty * dt2;
                    b[i]._positionZ += b[i]._velocityZ * dt + adtz * dt2;

                    //Calculate the new velocity of the body
                    b[i]._velocityX += adtx;
                    b[i]._velocityY += adty;
                    b[i]._velocityZ += adtz;

                    //Reset Forces
                    b[i]._forceX = 0; b[i]._forceY = 0; b[i]._forceZ = 0;
                }
            }
            //V9 is slower than V8
            public static void nextFrame3DV9(Body3D[] b, //Bodies
                double[] dtmass, //Table of the dt/mass for every body - used to calculate acceleration*dt(acceleration alone is not being used)
                double dt, //The dt
                double dt2, //The dt/2. Used to calculate acceleration/2*dt*dt = (acceleration*dt)*dt2
                double g //The constant G
                )
            {
                double dx, dy, dz; //used to find the connecting vector between two bodies. The subtract vector.
                double length2; //The square of the length of the subtract vector.
                double forceDivLength; //The force(newtons) of gravitation between the two bodies, divided by the length of the subtract vector. The subtract vector is multiplied by this value to generate the force vector.

                double adtx, adty, adtz; //The acceleration of the bodies multiplied by the dt(acceleration alone is not being used).
                for (int i = 1; i < b.Length - 1; i++)
                    for (int j = i + 1; j < b.Length; j++) //Run on every two bodies except for the first one
                    {
                        #region Calculate Force
                        {
                            //Generate subtract vector
                            dx = b[i]._positionX - b[j]._positionX;
                            dy = b[i]._positionY - b[j]._positionY;
                            dz = b[i]._positionZ - b[j]._positionZ;

                            //Generate force size
                            length2 = (dx * dx) + (dy * dy) + (dz * dz);
                            if (length2 == 0) forceDivLength = 0;
                            else forceDivLength = g * b[i]._mass * b[j]._mass / (length2 * Math.Sqrt(length2));

                            //Generate the force vector. Save it instead of the subtract vector
                            dx *= forceDivLength;
                            dy *= forceDivLength;
                            dz *= forceDivLength;

                            //Add the force vector to the bodies
                            b[i]._forceX -= dx; b[i]._forceY -= dy; b[i]._forceZ -= dz;
                            b[j]._forceX += dx; b[j]._forceY += dy; b[j]._forceZ += dz;
                        }
                        #endregion
                    }
                for (int i = 1; i < b.Length; i++) //Finish the force generating by running on every body with the first one, and then calculating the new position and velocity of that body.
                {
                    #region Calculate Force
                    {
                        //Generate subtract vector between the body and the first body
                        dx = b[i]._positionX - b[0]._positionX;
                        dy = b[i]._positionY - b[0]._positionY;
                        dz = b[i]._positionZ - b[0]._positionZ;

                        //Generate force size
                        length2 = (dx * dx) + (dy * dy) + (dz * dz);
                        if (length2 == 0) forceDivLength = 0;
                        else forceDivLength = g * b[i]._mass * b[0]._mass / (length2 * Math.Sqrt(length2));

                        //Generate the force vector. Save it instead of the subtract vector
                        dx *= forceDivLength;
                        dy *= forceDivLength;
                        dz *= forceDivLength;

                        //Add the force vector to the bodies
                        b[i]._forceX -= dx; b[i]._forceY -= dy; b[i]._forceZ -= dz;
                        b[0]._forceX += dx; b[0]._forceY += dy; b[0]._forceZ += dz;
                    }
                    #endregion
                    #region Calculate New Velocity and Position
                    {
                        //Calculate the acceleration of the body multiplied by dt
                        adtx = b[i]._forceX * dtmass[i];
                        adty = b[i]._forceY * dtmass[i];
                        adtz = b[i]._forceZ * dtmass[i];

                        //Calculate the new position of the body
                        b[i]._positionX += b[i]._velocityX * dt + adtx * dt2;
                        b[i]._positionY += b[i]._velocityY * dt + adty * dt2;
                        b[i]._positionZ += b[i]._velocityZ * dt + adtz * dt2;

                        //Calculate the new velocity of the body
                        b[i]._velocityX += adtx;
                        b[i]._velocityY += adty;
                        b[i]._velocityZ += adtz;

                        //Reset Forces
                        b[i]._forceX = 0; b[i]._forceY = 0; b[i]._forceZ = 0;
                    }
                    #endregion
                }
                #region Calculate New Velocity and Position
                {
                    //Calculate the acceleration of the first body multiplied by dt
                    adtx = b[0]._forceX * dtmass[0];
                    adty = b[0]._forceY * dtmass[0];
                    adtz = b[0]._forceZ * dtmass[0];

                    //Calculate the new position of the first body
                    b[0]._positionX += b[0]._velocityX * dt + adtx * dt2;
                    b[0]._positionY += b[0]._velocityY * dt + adty * dt2;
                    b[0]._positionZ += b[0]._velocityZ * dt + adtz * dt2;

                    //Calculate the new velocity of the first body
                    b[0]._velocityX += adtx;
                    b[0]._velocityY += adty;
                    b[0]._velocityZ += adtz;

                    //Reset Forces
                    b[0]._forceX = 0; b[0]._forceY = 0; b[0]._forceZ = 0;
                }
                #endregion
            }
            public static void nextFrame3DV10(Body3D[] b, //Bodies
                double[] dtmass, //Table of the dt/mass for every body - used to calculate acceleration*dt(acceleration alone is not being used)
                double[] massg, //Table of the g*mass for every body - used to calculate g*m1*m2
                double dt, //The dt
                double dt2 //The dt/2. Used to calculate acceleration/2*dt*dt = (acceleration*dt)*dt2
                )
            {
                double dx, dy, dz; //used to find the connecting vector between two bodies. The subtract vector.
                double length2; //The square of the length of the subtract vector.
                double forceDivLength; //The force(newtons) of gravitation between the two bodies, divided by the length of the subtract vector. The subtract vector is multiplied by this value to generate the force vector.

                double adtx, adty, adtz; //The acceleration of the bodies multiplied by the dt(acceleration alone is not being used).
                for (int i = 1; i < b.Length - 1; i++)
                    for (int j = i + 1; j < b.Length; j++) //Run on every two bodies except for the first one
                    {
                        #region Calculate Force
                        {
                            //Generate subtract vector
                            dx = b[i]._positionX - b[j]._positionX;
                            dy = b[i]._positionY - b[j]._positionY;
                            dz = b[i]._positionZ - b[j]._positionZ;

                            //Generate force size
                            length2 = (dx * dx) + (dy * dy) + (dz * dz);
                            if (length2 == 0) forceDivLength = 0;
                            else forceDivLength = massg[i] * b[j]._mass / (length2 * Math.Sqrt(length2));

                            //Generate the force vector. Save it instead of the subtract vector
                            dx *= forceDivLength;
                            dy *= forceDivLength;
                            dz *= forceDivLength;

                            //Add the force vector to the bodies
                            b[i]._forceX -= dx; b[i]._forceY -= dy; b[i]._forceZ -= dz;
                            b[j]._forceX += dx; b[j]._forceY += dy; b[j]._forceZ += dz;
                        }
                        #endregion
                    }
                for (int i = 1; i < b.Length; i++) //Finish the force generating by running on every body with the first one, and then calculating the new position and velocity of that body.
                {
                    #region Calculate Force
                    {
                        //Generate subtract vector between the body and the first body
                        dx = b[i]._positionX - b[0]._positionX;
                        dy = b[i]._positionY - b[0]._positionY;
                        dz = b[i]._positionZ - b[0]._positionZ;

                        //Generate force size
                        length2 = (dx * dx) + (dy * dy) + (dz * dz);
                        if (length2 == 0) forceDivLength = 0;
                        else forceDivLength = massg[i] * b[0]._mass / (length2 * Math.Sqrt(length2));

                        //Generate the force vector. Save it instead of the subtract vector
                        dx *= forceDivLength;
                        dy *= forceDivLength;
                        dz *= forceDivLength;

                        //Add the force vector to the bodies
                        b[i]._forceX -= dx; b[i]._forceY -= dy; b[i]._forceZ -= dz;
                        b[0]._forceX += dx; b[0]._forceY += dy; b[0]._forceZ += dz;
                    }
                    #endregion
                    #region Calculate New Velocity and Position
                    {
                        //Calculate the acceleration of the body multiplied by dt
                        adtx = b[i]._forceX * dtmass[i];
                        adty = b[i]._forceY * dtmass[i];
                        adtz = b[i]._forceZ * dtmass[i];

                        //Calculate the new position of the body
                        b[i]._positionX += b[i]._velocityX * dt + adtx * dt2;
                        b[i]._positionY += b[i]._velocityY * dt + adty * dt2;
                        b[i]._positionZ += b[i]._velocityZ * dt + adtz * dt2;

                        //Calculate the new velocity of the body
                        b[i]._velocityX += adtx;
                        b[i]._velocityY += adty;
                        b[i]._velocityZ += adtz;

                        //Reset Forces
                        b[i]._forceX = 0; b[i]._forceY = 0; b[i]._forceZ = 0;
                    }
                    #endregion
                }
                #region Calculate New Velocity and Position
                {
                    //Calculate the acceleration of the first body multiplied by dt
                    adtx = b[0]._forceX * dtmass[0];
                    adty = b[0]._forceY * dtmass[0];
                    adtz = b[0]._forceZ * dtmass[0];

                    //Calculate the new position of the first body
                    b[0]._positionX += b[0]._velocityX * dt + adtx * dt2;
                    b[0]._positionY += b[0]._velocityY * dt + adty * dt2;
                    b[0]._positionZ += b[0]._velocityZ * dt + adtz * dt2;

                    //Calculate the new velocity of the first body
                    b[0]._velocityX += adtx;
                    b[0]._velocityY += adty;
                    b[0]._velocityZ += adtz;

                    //Reset Forces
                    b[0]._forceX = 0; b[0]._forceY = 0; b[0]._forceZ = 0;
                }
                #endregion
            }
        #endregion
        private void RunNextFrameVersion(int v, bool twoDimensional)
        {
            if (twoDimensional)
            {
                switch (v)
                {
                    case 0: nextFrame2DV0(this.bodies2D, this.dt, this.g); break;
                    case 1: nextFrame2DV1(this.bodies2D, this.dt, this.g); break;
                    case 2: nextFrame2DV2(this.bodies2D, this.dt, this.g); break;
                    case 3: nextFrame2DV3(this.bodies2D, this.dt, this.g); break;
                    case 4: nextFrame2DV4(this.bodies2D, this.dt, this.dt_div_2, this.g); break;
                    case 5: nextFrame2DV5(this.bodies2D, this.num_of_bodies, this.dt, this.dt_div_2, this.g); break;
                    case 6: nextFrame2DV6(this.bodies2D, this.mass_inverse, this.dt, this.dt_div_2, this.g); break;
                    case 7: nextFrame2DV7(this.bodies2D, this.dt_div_mass, this.dt, this.dt_div_2, this.g); break;
                    case 8: nextFrame2DV8(this.bodies2D, this.dt_div_mass, this.g_m1_m2, this.dt, this.dt_div_2); break;
                    case 9: nextFrame2DV9(this.bodies2D, this.dt_div_mass, this.dt, this.dt_div_2, this.g); break;
                    case 10: nextFrame2DV10(this.bodies2D, this.dt_div_mass, this.mass_mul_g, this.dt, this.dt_div_2); break;
                    default: return;
                }
            }
            else
            {
                switch (v)
                {
                    case 0: nextFrame3DV0(this.bodies3D, this.dt, this.g); break;
                    case 1: nextFrame3DV1(this.bodies3D, this.dt, this.g); break;
                    case 2: nextFrame3DV2(this.bodies3D, this.dt, this.g); break;
                    case 3: nextFrame3DV3(this.bodies3D, this.dt, this.g); break;
                    case 4: nextFrame3DV4(this.bodies3D, this.dt, this.dt_div_2, this.g); break;
                    case 5: nextFrame3DV5(this.bodies3D, this.num_of_bodies, this.dt, this.dt_div_2, this.g); break;
                    case 6: nextFrame3DV6(this.bodies3D, this.mass_inverse, this.dt, this.dt_div_2, this.g); break;
                    case 7: nextFrame3DV7(this.bodies3D, this.dt_div_mass, this.dt, this.dt_div_2, this.g); break;
                    case 8: nextFrame3DV8(this.bodies3D, this.dt_div_mass, this.g_m1_m2, this.dt, this.dt_div_2); break;
                    case 9: nextFrame3DV9(this.bodies3D, this.dt_div_mass, this.dt, this.dt_div_2, this.g); break;
                    case 10: nextFrame3DV10(this.bodies3D, this.dt_div_mass, this.mass_mul_g, this.dt, this.dt_div_2); break;
                    default: return;
                }
            }
        }
        private double GetEner()
        {
            double ener = 0;
            if (this.two_dimensional)
            {
                for (int i = 0; i < this.bodies2D.Length; i++)
                    ener += this.bodies2D[i]._mass * (this.bodies2D[i]._velocityX * this.bodies2D[i]._velocityX + this.bodies2D[i]._velocityY * this.bodies2D[i]._velocityY);
                ener /= 2;
                for (int i = 0; i < this.bodies2D.Length - 1; i++)
                    for (int j = i + 1; j < this.bodies2D.Length; j++)
                        ener -= 2 * this.g_m1_m2[i,j] / Math.Sqrt((this.bodies2D[i]._positionX - this.bodies2D[j]._positionX) * (this.bodies2D[i]._positionX - this.bodies2D[j]._positionX) + (this.bodies2D[i]._positionY - this.bodies2D[j]._positionY) * (this.bodies2D[i]._positionY - this.bodies2D[j]._positionY));
            }
            else
            {
                for (int i = 0; i < this.bodies3D.Length; i++)
                    ener += this.bodies3D[i]._mass * (this.bodies3D[i]._velocityX * this.bodies3D[i]._velocityX + this.bodies3D[i]._velocityY * this.bodies3D[i]._velocityY + this.bodies3D[i]._velocityZ * this.bodies3D[i]._velocityZ);
                ener /= 2;
                for (int i = 0; i < this.bodies3D.Length - 1; i++)
                    for (int j = i + 1; j < this.bodies3D.Length; j++)
                        ener -= 2 * this.g_m1_m2[i, j] / Math.Sqrt((this.bodies3D[i]._positionX - this.bodies3D[j]._positionX) * (this.bodies3D[i]._positionX - this.bodies3D[j]._positionX) + (this.bodies3D[i]._positionY - this.bodies3D[j]._positionY) * (this.bodies3D[i]._positionY - this.bodies3D[j]._positionY) + (this.bodies3D[i]._positionZ - this.bodies3D[j]._positionZ) * (this.bodies3D[i]._positionZ - this.bodies3D[j]._positionZ));
            }
            return ener;
        }

        #region Speed Testing
        private List<TimeSpan> GenerateTimings(bool twoDimensional)
            {
                Stopwatch timer = new Stopwatch();
                List<TimeSpan> timings = new List<TimeSpan>();
                for (int v = 0; v <= this.max_version; v++)
                {
                    timer.Start();
                    ulong mod = this.speed_tester_rounds / 100;
                    Console.Title = "N-Body Simulator Speed Tester - Currently testing " + (twoDimensional ? "2D" : "3D") + "V" + v;
                    for (ulong i = 0; i < this.speed_tester_rounds; i++)
                    {
                        this.RunNextFrameVersion(v, twoDimensional);
                    }
                    timer.Stop();
                    timings.Add(timer.Elapsed);
                    timer.Reset();
                }
                return timings;
            }
            private string GenerateTimingsLog(List<TimeSpan> timings)
            {
                string ret = "";
                string maxVerFormat = "{0:" + new String('0', this.NumberOfDigits(this.max_version)) + "}";
                for (int i = 0; i < timings.Count; i++)
                {
                    if (i != 0) ret += "\r\n";
                    ret += "V" + string.Format(maxVerFormat, i) + " Timing: " + timings[i].ToString();
                }
                return ret;
            }
            private string GenerateCPSLog(List<TimeSpan> timings)
            {
                string ret = "";
                double maxcps = 0;
                for (int i = 0; i < timings.Count; i++)
                    if ((double)this.speed_tester_rounds / timings[i].TotalSeconds > maxcps)
                        maxcps = (double)this.speed_tester_rounds / timings[i].TotalSeconds;
                string maxSpeedFormat = "{0:" + new String('0', this.NumberOfDigits((int)Math.Ceiling(maxcps))) + "}";
                string maxVerFormat = "{0:" + new String('0', this.NumberOfDigits(this.max_version)) + "}";
                for (int i = 0; i < timings.Count; i++)
                {
                    if (i != 0) ret += "\r\n";
                    ret += "V" + string.Format(maxVerFormat, i) + " Operations per Second: " + string.Format(maxSpeedFormat, (double)this.speed_tester_rounds / timings[i].TotalSeconds);
                }
                return ret;
            }
            private int NumberOfDigits(int num)
            {
                int res = 0;
                while (num != 0)
                {
                    num /= 10;
                    res++;
                }
                return res;
            }
        #endregion
    }

    public struct Body2D
    {
        public double _positionX;
        public double _positionY;
        public double _velocityX;
        public double _velocityY;
        public double _forceX;
        public double _forceY;
        public double _mass;
        public Body2D(double positionX, double positionY, double velocityX, double velocityY, double mass)
        {
            this._positionX = positionX;
            this._positionY = positionY;
            this._velocityX = velocityX;
            this._velocityY = velocityY;
            this._forceX = 0;
            this._forceY = 0;
            this._mass = mass;
        }
    }
    public struct Body3D
    {
        public double _positionX;
        public double _positionY;
        public double _positionZ;
        public double _velocityX;
        public double _velocityY;
        public double _velocityZ;
        public double _forceX;
        public double _forceY;
        public double _forceZ;
        public double _mass;
        public Body3D(double positionX, double positionY, double positionZ, double velocityX, double velocityY, double velocityZ, double mass)
        {
            this._positionX = positionX;
            this._positionY = positionY;
            this._positionZ = positionZ;
            this._velocityX = velocityX;
            this._velocityY = velocityY;
            this._velocityZ = velocityZ;
            this._forceX = 0;
            this._forceY = 0;
            this._forceZ = 0;
            this._mass = mass;
        }
    }
    public struct VisualProp
    {
        public double _radius;
        public double _trailradius;
        public Color _color;
        public Color _trail;
        public VisualProp(double radius, double trailradius, Color color, Color trail)
        {
            this._radius = radius;
            this._trailradius = trailradius;
            this._color = color;
            this._trail = trail;
        }
    }
    public struct TrailDot
    {
        public double _x;
        public double _y;
        public double _radius;
        public Color _color;
        public TrailDot(double x, double y, double radius, Color color)
        {
            this._x = x;
            this._y = y;
            this._color = color;
            this._radius = radius;
        }
    }
}
