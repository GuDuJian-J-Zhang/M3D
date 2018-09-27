package com.example.svllib;

public class SVLLibTest {
	static{
		System.loadLibrary("SVLLib");
	}    
	public native void Test();
	public static void main(String[] args) {
        System.loadLibrary("SVLLib");
        SVLLibTest nativeCode=new SVLLibTest();
        nativeCode.Test();
        }
}
