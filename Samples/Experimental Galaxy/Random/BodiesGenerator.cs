using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace NBodyBodiesGenerator
{
    class Program
    {
        static void Main(string[] args)
        {
            List<Body3D> l = new List<Body3D>(1000);
            Random rnd = new Random();
            string fileLocation = "C:\\bodies.dat";
            int bodies = 1000;
            double g = 6.673848E-11;
            double totalMass = 2E42;
            double radius = (double)9460730472580800 * 55000;
            double width = (double)9460730472580800 * 1000;
            double bodyRadius = (double)9460730472580800 * 100;
            for (int i = 0; i < bodies; i++)
            {
                Body3D b = new Body3D();
                b.Name = "Body #" + i.ToString();
                b.Color = Color.White;
                b.TrailColor = Color.Gray;
                b.Radius = bodyRadius;
                b.TrailWidth = bodyRadius;
                b.Mass = rnd.NextDouble() * 0.2 * (totalMass / bodies) + 0.9 * (totalMass / bodies); //between 90% and 110% of (totalMass/bodies)
                double dist = rnd.NextDouble() * 2 * radius - radius; //between -radius and radius
                double angle = rnd.NextDouble() * 2 * Math.PI;
                b.PositionX = dist * Math.Cos(angle);
                b.PositionY = dist * Math.Sin(angle);
                b.PositionZ = rnd.NextDouble() * width - width/2; //between -width/2 and width/2
                b.VelocityX = 0;
                b.VelocityY = 0;
                b.VelocityZ = 0;
                l.Add(b);
            }
            System.IO.File.WriteAllBytes(fileLocation, Body3D.ListToBytes(l, g).ToArray());
        }
    }
}
