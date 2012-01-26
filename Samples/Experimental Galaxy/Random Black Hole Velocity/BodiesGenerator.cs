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
            Body3D bh = new Body3D();
            bh.Name = "Black Hole";
            bh.Color = Color.Yellow;
            bh.TrailColor = Color.YellowGreen;
            bh.Radius = bodyRadius * 10;
            bh.TrailWidth = bodyRadius;
            bh.Mass = (rnd.NextDouble() * 0.2 * (totalMass / bodies) + 0.9 * (totalMass / bodies)) * 4100000;
            bh.PositionX = 0;
            bh.PositionY = 0;
            bh.PositionZ = 0;
            bh.VelocityX = 0;
            bh.VelocityY = 0;
            bh.VelocityZ = 0;
            l.Add(bh);
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
                b.PositionZ = rnd.NextDouble() * width - width / 2; //between -width/2 and width/2
                b.VelocityX = 0;
                b.VelocityY = 0;
                b.VelocityZ = 0;
                l.Add(b);
            }
            double dRX, dRY, dRZ; //used to find the connecting vector between two bodies. The Rji vector.
            double length_pow_3; //The third power of the length of the Rji vector.
            double force_div_length; //The force(newtons) of gravitation between the two bodies, divided by the length of the Rji vector. The Rji vector is multiplied by this value to generate the force vector.
            for (int i = 0; i < bodies - 1; i++)
                for (int j = i + 1; j < bodies; j++) //Run on every two bodies
                {
                    //Generate Rji vector
                    dRX = l[j].PositionX - l[i].PositionX;
                    dRY = l[j].PositionY - l[i].PositionY;
                    dRZ = l[j].PositionZ - l[i].PositionZ;

                    //Generate force size
                    length_pow_3 = (dRX * dRX) + (dRY * dRY) + (dRZ * dRZ); //Length squared
                    length_pow_3 = length_pow_3 * Math.Sqrt(length_pow_3);
                    if (length_pow_3 == 0) force_div_length = 0;
                    else force_div_length = g * l[i].Mass * l[j].Mass / length_pow_3;

                    //Generate the force vector. Save it instead of the Rji vector
                    dRX *= force_div_length;
                    dRY *= force_div_length;
                    dRZ *= force_div_length;

                    //Add the force vector to the bodies(temporarily instead of the velocity)
                    l[i].VelocityX += dRX; l[i].VelocityY += dRY; l[i].VelocityZ += dRZ;
                    l[j].VelocityX -= dRX; l[j].VelocityY -= dRY; l[j].VelocityZ -= dRZ;
                }
            double PX, PY, PZ; //Vector force
            double FX, FY, FZ; //Vector force
            double FcX, FcY, FcZ; //Vector force to center
            double TvX, TvY, TvZ; //Temp Velocity vector
            double dotProd;
            double length_squared;
            for (int i = 1; i < bodies; i++)
            {
                FX = l[i].VelocityX; PX = l[i].PositionX;
                FY = l[i].VelocityY; PY = l[i].PositionY;
                FZ = l[i].VelocityZ; PZ = l[i].PositionZ;
                length_squared = PX * PX + PY * PY + PZ * PZ;
                dotProd = PX * FX + PY * FY + PZ * FZ;
                //dotProd = g * l[i].Mass * l[0].Mass / length_squared;
                FcX = (dotProd / length_squared) * PX;
                FcY = (dotProd / length_squared) * PY;
                FcZ = (dotProd / length_squared) * PZ;
                TvX = -FcY;
                TvY = FcX;
                TvZ = 0;
                length_squared = TvX * TvX + TvY * TvY + TvZ * TvZ;
                TvX /= Math.Sqrt(length_squared);
                TvY /= Math.Sqrt(length_squared);
                TvZ /= Math.Sqrt(length_squared);
                length_squared = l[i].PositionX * l[i].PositionX + l[i].PositionY * l[i].PositionY;
                length_squared *= FcX * FcX + FcY * FcY;
                l[i].VelocityX = TvX * Math.Sqrt(Math.Sqrt(length_squared) / l[i].Mass);
                l[i].VelocityY = TvY * Math.Sqrt(Math.Sqrt(length_squared) / l[i].Mass);
                l[i].VelocityZ = TvZ * Math.Sqrt(Math.Sqrt(length_squared) / l[i].Mass);
            }
            l[0].VelocityX = 0;
            l[0].VelocityY = 0;
            l[0].VelocityZ = 0;
            System.IO.File.WriteAllBytes(fileLocation, Body3D.ListToBytes(l, g).ToArray());
        }
    }
}