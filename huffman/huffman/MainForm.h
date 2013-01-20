// Copyright (c) Gratian Lup. All rights reserved.
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
//       copyright notice, this list of conditions and the following
//       disclaimer in the documentation and/or other materials provided
//       with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#pragma once

#include "Huffman.hpp"

namespace huffman {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
    using namespace System::IO;
    using namespace System::Runtime::InteropServices;

	/// <summary>
	/// Summary for Form1
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
    private: System::Windows::Forms::Button^  button2;
    protected: 
    private: System::Windows::Forms::Label^  label2;
    private: System::Windows::Forms::Label^  label1;
    private: System::Windows::Forms::Button^  button4;
    private: System::Windows::Forms::Button^  button3;
    private: System::Windows::Forms::TextBox^  textBox2;
    private: System::Windows::Forms::TextBox^  textBox1;
    private: System::Windows::Forms::Button^  button1;
    private: System::Windows::Forms::OpenFileDialog^  openFileDialog1;
    private: System::Windows::Forms::OpenFileDialog^  openFileDialog2;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
            this->button2 = (gcnew System::Windows::Forms::Button());
            this->label2 = (gcnew System::Windows::Forms::Label());
            this->label1 = (gcnew System::Windows::Forms::Label());
            this->button4 = (gcnew System::Windows::Forms::Button());
            this->button3 = (gcnew System::Windows::Forms::Button());
            this->textBox2 = (gcnew System::Windows::Forms::TextBox());
            this->textBox1 = (gcnew System::Windows::Forms::TextBox());
            this->button1 = (gcnew System::Windows::Forms::Button());
            this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
            this->openFileDialog2 = (gcnew System::Windows::Forms::OpenFileDialog());
            this->SuspendLayout();
            // 
            // button2
            // 
            this->button2->Location = System::Drawing::Point(320, 82);
            this->button2->Name = L"button2";
            this->button2->Size = System::Drawing::Size(91, 36);
            this->button2->TabIndex = 37;
            this->button2->Text = L"Decompress";
            this->button2->UseVisualStyleBackColor = true;
            this->button2->Click += gcnew System::EventHandler(this, &Form1::button2_Click);
            // 
            // label2
            // 
            this->label2->AutoSize = true;
            this->label2->Location = System::Drawing::Point(12, 42);
            this->label2->Name = L"label2";
            this->label2->Size = System::Drawing::Size(60, 13);
            this->label2->TabIndex = 36;
            this->label2->Text = L"Destination";
            // 
            // label1
            // 
            this->label1->AutoSize = true;
            this->label1->Location = System::Drawing::Point(12, 15);
            this->label1->Name = L"label1";
            this->label1->Size = System::Drawing::Size(41, 13);
            this->label1->TabIndex = 35;
            this->label1->Text = L"Source";
            // 
            // button4
            // 
            this->button4->Location = System::Drawing::Point(336, 36);
            this->button4->Name = L"button4";
            this->button4->Size = System::Drawing::Size(75, 23);
            this->button4->TabIndex = 34;
            this->button4->Text = L"Browse";
            this->button4->UseVisualStyleBackColor = true;
            this->button4->Click += gcnew System::EventHandler(this, &Form1::button4_Click);
            // 
            // button3
            // 
            this->button3->Location = System::Drawing::Point(336, 9);
            this->button3->Name = L"button3";
            this->button3->Size = System::Drawing::Size(75, 23);
            this->button3->TabIndex = 33;
            this->button3->Text = L"Browse";
            this->button3->UseVisualStyleBackColor = true;
            this->button3->Click += gcnew System::EventHandler(this, &Form1::button3_Click);
            // 
            // textBox2
            // 
            this->textBox2->AutoCompleteSource = System::Windows::Forms::AutoCompleteSource::FileSystem;
            this->textBox2->Location = System::Drawing::Point(78, 38);
            this->textBox2->Name = L"textBox2";
            this->textBox2->Size = System::Drawing::Size(252, 20);
            this->textBox2->TabIndex = 32;
            // 
            // textBox1
            // 
            this->textBox1->AutoCompleteSource = System::Windows::Forms::AutoCompleteSource::FileSystem;
            this->textBox1->Location = System::Drawing::Point(77, 12);
            this->textBox1->Name = L"textBox1";
            this->textBox1->Size = System::Drawing::Size(253, 20);
            this->textBox1->TabIndex = 31;
            // 
            // button1
            // 
            this->button1->Location = System::Drawing::Point(223, 82);
            this->button1->Name = L"button1";
            this->button1->Size = System::Drawing::Size(91, 36);
            this->button1->TabIndex = 30;
            this->button1->Text = L"Compress";
            this->button1->UseVisualStyleBackColor = true;
            this->button1->Click += gcnew System::EventHandler(this, &Form1::button1_Click);
            // 
            // openFileDialog1
            // 
            this->openFileDialog1->FileName = L"openFileDialog1";
            // 
            // openFileDialog2
            // 
            this->openFileDialog2->FileName = L"openFileDialog2";
            // 
            // Form1
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(423, 133);
            this->Controls->Add(this->button2);
            this->Controls->Add(this->label2);
            this->Controls->Add(this->label1);
            this->Controls->Add(this->button4);
            this->Controls->Add(this->button3);
            this->Controls->Add(this->textBox2);
            this->Controls->Add(this->textBox1);
            this->Controls->Add(this->button1);
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedToolWindow;
            this->Name = L"Form1";
            this->Text = L"Huffman";
            this->ResumeLayout(false);
            this->PerformLayout();

        }
#pragma endregion
    private: System::Void button3_Click(System::Object^  sender, System::EventArgs^  e) {
                 if(openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
                     textBox1->Text = openFileDialog1->FileName;

                     String^ dir = Path::GetDirectoryName(openFileDialog1->FileName);
                     textBox2->Text = Path::Combine(dir, Path::GetFileName(openFileDialog1->FileName) + ".huff");
                 }
             }
private: System::Void button4_Click(System::Object^  sender, System::EventArgs^  e) {
             if(openFileDialog2->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
                 textBox2->Text = openFileDialog1->FileName;
             }
         }
private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
             Huffman huff;
             char* from = (char*)(void*)Marshal::StringToHGlobalAnsi(textBox1->Text);
             char* to = (char*)(void*)Marshal::StringToHGlobalAnsi(textBox2->Text);

             huff.Compress(from, to);
             MessageBox::Show("File compressed successfully");
         }
private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) {
             Huffman huff;
             char* from = (char*)(void*)Marshal::StringToHGlobalAnsi(textBox1->Text);
             char* to = (char*)(void*)Marshal::StringToHGlobalAnsi(textBox2->Text);

             huff.Decompress(from, to);
             MessageBox::Show("File decompressed successfully");
         }
};
}

