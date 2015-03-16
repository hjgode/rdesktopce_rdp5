using System;

using System.Collections.Generic;
using System.Text;

using System.Runtime.InteropServices;
using System.Diagnostics;

namespace RDPman
{
    public class win32
    {
        [DllImport("coredll.dll", EntryPoint = ("GetSystemMetrics"))]
        public static extern int GetSystemMetrics(int nIndex);

        private const int SM_CXSCREEN = 0;
        private const int SM_CYSCREEN = 1;

        public static int ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
        public static int ScreenHeight = GetSystemMetrics(SM_CYSCREEN);

        public static bool startApp(string sApp, string sArg){
            bool bRet = false;
            if (!System.IO.File.Exists(sApp))
                return bRet;
            Process _process = new Process();
            _process.StartInfo.Arguments = sArg;
            _process.StartInfo.FileName = sApp;
            try
            {
                _process.Start();
                System.Threading.Thread.Sleep(200);
                if(_process.Id!=0)
                //if(_process.MainWindowHandle!=IntPtr.Zero) //this will not work as rdesktopce main win will be delayed
                    bRet = true;
            }
            catch (Exception) { }
            return bRet;
        }

    }
}
