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

#include "LZ77.hpp"

namespace lz77 {

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
    private: System::Windows::Forms::Button^  button1;
    protected: 
    private: System::Windows::Forms::TextBox^  textBox1;
    private: System::Windows::Forms::TextBox^  textBox2;
    private: System::Windows::Forms::Button^  button3;
    private: System::Windows::Forms::Button^  button4;
    private: System::Windows::Forms::Label^  label1;
    private: System::Windows::Forms::Label^  label2;
    private: System::Windows::Forms::Button^  button2;
    private: System::Windows::Forms::Label^  label3;
    private: System::Windows::Forms::Label^  label4;
    private: System::Windows::Forms::TrackBar^  trackBar1;
    private: System::Windows::Forms::TrackBar^  trackBar2;
    private: System::Windows::Forms::Label^  label5;
    private: System::Windows::Forms::Label^  label6;
    private: System::Windows::Forms::OpenFileDialog^  openFileDialog1;
    private: System::Windows::Forms::OpenFileDialog^  openFileDialog2;
    private: System::Windows::Forms::TextBox^  textBox3;
    private: System::Windows::Forms::CheckBox^  checkBox1;

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
            this->button1 = (gcnew System::Windows::Forms::Button());
            this->textBox1 = (gcnew System::Windows::Forms::TextBox());
            this->textBox2 = (gcnew System::Windows::Forms::TextBox());
            this->button3 = (gcnew System::Windows::Forms::Button());
            this->button4 = (gcnew System::Windows::Forms::Button());
            this->label1 = (gcnew System::Windows::Forms::Label());
            this->label2 = (gcnew System::Windows::Forms::Label());
            this->button2 = (gcnew System::Windows::Forms::Button());
            this->label3 = (gcnew System::Windows::Forms::Label());
            this->label4 = (gcnew System::Windows::Forms::Label());
            this->trackBar1 = (gcnew System::Windows::Forms::TrackBar());
            this->trackBar2 = (gcnew System::Windows::Forms::TrackBar());
            this->label5 = (gcnew System::Windows::Forms::Label());
            this->label6 = (gcnew System::Windows::Forms::Label());
            this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
            this->openFileDialog2 = (gcnew System::Windows::Forms::OpenFileDialog());
            this->textBox3 = (gcnew System::Windows::Forms::TextBox());
            this->checkBox1 = (gcnew System::Windows::Forms::CheckBox());
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->trackBar1))->BeginInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->trackBar2))->BeginInit();
            this->SuspendLayout();
            // 
            // button1
            // 
            this->button1->Location = System::Drawing::Point(223, 82);
            this->button1->Name = L"button1";
            this->button1->Size = System::Drawing::Size(91, 36);
            this->button1->TabIndex = 0;
            this->button1->Text = L"Compress";
            this->button1->UseVisualStyleBackColor = true;
            this->button1->Click += gcnew System::EventHandler(this, &Form1::button1_Click);
            // 
            // textBox1
            // 
            this->textBox1->AutoCompleteSource = System::Windows::Forms::AutoCompleteSource::FileSystem;
            this->textBox1->Location = System::Drawing::Point(77, 12);
            this->textBox1->Name = L"textBox1";
            this->textBox1->Size = System::Drawing::Size(253, 20);
            this->textBox1->TabIndex = 2;
            // 
            // textBox2
            // 
            this->textBox2->AutoCompleteSource = System::Windows::Forms::AutoCompleteSource::FileSystem;
            this->textBox2->Location = System::Drawing::Point(78, 38);
            this->textBox2->Name = L"textBox2";
            this->textBox2->Size = System::Drawing::Size(252, 20);
            this->textBox2->TabIndex = 3;
            // 
            // button3
            // 
            this->button3->Location = System::Drawing::Point(336, 9);
            this->button3->Name = L"button3";
            this->button3->Size = System::Drawing::Size(75, 23);
            this->button3->TabIndex = 4;
            this->button3->Text = L"Browse";
            this->button3->UseVisualStyleBackColor = true;
            this->button3->Click += gcnew System::EventHandler(this, &Form1::button3_Click);
            // 
            // button4
            // 
            this->button4->Location = System::Drawing::Point(336, 36);
            this->button4->Name = L"button4";
            this->button4->Size = System::Drawing::Size(75, 23);
            this->button4->TabIndex = 5;
            this->button4->Text = L"Browse";
            this->button4->UseVisualStyleBackColor = true;
            this->button4->Click += gcnew System::EventHandler(this, &Form1::button4_Click);
            // 
            // label1
            // 
            this->label1->AutoSize = true;
            this->label1->Location = System::Drawing::Point(12, 15);
            this->label1->Name = L"label1";
            this->label1->Size = System::Drawing::Size(41, 13);
            this->label1->TabIndex = 6;
            this->label1->Text = L"Source";
            // 
            // label2
            // 
            this->label2->AutoSize = true;
            this->label2->Location = System::Drawing::Point(12, 42);
            this->label2->Name = L"label2";
            this->label2->Size = System::Drawing::Size(60, 13);
            this->label2->TabIndex = 7;
            this->label2->Text = L"Destination";
            // 
            // button2
            // 
            this->button2->Location = System::Drawing::Point(320, 82);
            this->button2->Name = L"button2";
            this->button2->Size = System::Drawing::Size(91, 36);
            this->button2->TabIndex = 8;
            this->button2->Text = L"Decompress";
            this->button2->UseVisualStyleBackColor = true;
            this->button2->Click += gcnew System::EventHandler(this, &Form1::button2_Click);
            // 
            // label3
            // 
            this->label3->AutoSize = true;
            this->label3->Location = System::Drawing::Point(12, 81);
            this->label3->Name = L"label3";
            this->label3->Size = System::Drawing::Size(35, 13);
            this->label3->TabIndex = 9;
            this->label3->Text = L"Offset";
            // 
            // label4
            // 
            this->label4->AutoSize = true;
            this->label4->Location = System::Drawing::Point(12, 108);
            this->label4->Name = L"label4";
            this->label4->Size = System::Drawing::Size(27, 13);
            this->label4->TabIndex = 10;
            this->label4->Text = L"Size";
            // 
            // trackBar1
            // 
            this->trackBar1->Location = System::Drawing::Point(77, 73);
            this->trackBar1->Maximum = 16;
            this->trackBar1->Minimum = 3;
            this->trackBar1->Name = L"trackBar1";
            this->trackBar1->Size = System::Drawing::Size(104, 45);
            this->trackBar1->TabIndex = 11;
            this->trackBar1->Value = 3;
            this->trackBar1->Scroll += gcnew System::EventHandler(this, &Form1::trackBar1_Scroll);
            // 
            // trackBar2
            // 
            this->trackBar2->Location = System::Drawing::Point(78, 105);
            this->trackBar2->Maximum = 5;
            this->trackBar2->Minimum = 2;
            this->trackBar2->Name = L"trackBar2";
            this->trackBar2->Size = System::Drawing::Size(104, 45);
            this->trackBar2->TabIndex = 12;
            this->trackBar2->Value = 2;
            this->trackBar2->Scroll += gcnew System::EventHandler(this, &Form1::trackBar2_Scroll);
            // 
            // label5
            // 
            this->label5->AutoSize = true;
            this->label5->Location = System::Drawing::Point(187, 79);
            this->label5->Name = L"label5";
            this->label5->Size = System::Drawing::Size(13, 13);
            this->label5->TabIndex = 13;
            this->label5->Text = L"3";
            // 
            // label6
            // 
            this->label6->AutoSize = true;
            this->label6->Location = System::Drawing::Point(187, 106);
            this->label6->Name = L"label6";
            this->label6->Size = System::Drawing::Size(13, 13);
            this->label6->TabIndex = 14;
            this->label6->Text = L"2";
            // 
            // openFileDialog1
            // 
            this->openFileDialog1->FileName = L"openFileDialog1";
            // 
            // openFileDialog2
            // 
            this->openFileDialog2->FileName = L"openFileDialog2";
            // 
            // textBox3
            // 
            this->textBox3->Location = System::Drawing::Point(15, 173);
            this->textBox3->Multiline = true;
            this->textBox3->Name = L"textBox3";
            this->textBox3->Size = System::Drawing::Size(396, 222);
            this->textBox3->TabIndex = 15;
            // 
            // checkBox1
            // 
            this->checkBox1->AutoSize = true;
            this->checkBox1->Location = System::Drawing::Point(15, 150);
            this->checkBox1->Name = L"checkBox1";
            this->checkBox1->Size = System::Drawing::Size(92, 17);
            this->checkBox1->TabIndex = 16;
            this->checkBox1->Text = L"Show Tokens";
            this->checkBox1->UseVisualStyleBackColor = true;
            // 
            // Form1
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(425, 407);
            this->Controls->Add(this->checkBox1);
            this->Controls->Add(this->textBox3);
            this->Controls->Add(this->label6);
            this->Controls->Add(this->label5);
            this->Controls->Add(this->trackBar2);
            this->Controls->Add(this->trackBar1);
            this->Controls->Add(this->label4);
            this->Controls->Add(this->label3);
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
            this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
            this->Text = L"LZ77";
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->trackBar1))->EndInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->trackBar2))->EndInit();
            this->ResumeLayout(false);
            this->PerformLayout();

        }
#pragma endregion
    private: System::Void trackBar1_Scroll(System::Object^  sender, System::EventArgs^  e) {
                 label5->Text = trackBar1->Value.ToString();
             }
private: System::Void trackBar2_Scroll(System::Object^  sender, System::EventArgs^  e) {
             label6->Text = trackBar2->Value.ToString();
         }
private: System::Void button3_Click(System::Object^  sender, System::EventArgs^  e) {
             if(openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
                 textBox1->Text = openFileDialog1->FileName;

                 String^ dir = Path::GetDirectoryName(openFileDialog1->FileName);
                 String^ details = ".o" + label5->Text + "l" + label6->Text;
                 textBox2->Text = Path::Combine(dir, Path::GetFileName(openFileDialog1->FileName) +  details + ".lz77");
             }
         }
private: System::Void button4_Click(System::Object^  sender, System::EventArgs^  e) {
             if(openFileDialog2->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
                 textBox2->Text = openFileDialog1->FileName;
             }
         }
private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
             LZ77 lz(1024*1024, checkBox1->Checked);

             char* from = (char*)(void*)Marshal::StringToHGlobalAnsi(textBox1->Text);
             char* to = (char*)(void*)Marshal::StringToHGlobalAnsi(textBox2->Text);

             lz.Compress(from, to, trackBar1->Value, trackBar2->Value);
             MessageBox::Show("File compressed successfully");

             if(checkBox1->Checked) {
                 textBox3->Text = gcnew String(lz.TokenLog().c_str());
             }
         }
private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) {
             LZ77 lz(1024*1024, checkBox1->Checked);

             char* from = (char*)(void*)Marshal::StringToHGlobalAnsi(textBox1->Text);
             char* to = (char*)(void*)Marshal::StringToHGlobalAnsi(textBox2->Text);

             lz.Decompress(from, to);
             MessageBox::Show("File decompressed successfully");

             if(checkBox1->Checked) {
                 textBox3->Text = gcnew String(lz.TokenLog().c_str());
             }
         }
};
}

