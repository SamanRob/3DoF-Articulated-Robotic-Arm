from tkinter import *
import serial
#from PIL import ImageTk, Image


commPort= "COM7"
ser = serial.Serial(commPort, baudrate=115200)
root = Tk()
#root.title("RobotARM")
#root.geometry("800x1200")
#root.iconbitmap("ICONICO.ico")
#my_img = Image.open("Arm.png")
#resized = my_img.resize((100,400))
#new_pic = ImageTk.PhotoImage(resized)
#my_label = Label(image=new_pic)
#my_label.grid(row=17, column=1)
#my_label= Label(root, text='ROBOT ORIENTATION IN HOME POSITION MUST BE LIKE THAT')
#my_label.grid(row=17, column=1)


my_entries = []
def Fill_in_Data():
    entry_list = ''
    for entries in my_entries:
        entry_list= entry_list + str(entries.get())+ ','
        #my_label1.config(text=entry_list)
    msg = entry_list.encode()
    ser.write(msg)
    print(msg)  # this shows how the encoded list looks like

for x in range(3):
    my_entry = Entry(root)
    my_entry.insert(END,'')
    my_entry.grid(row=2, column=x, pady=10, padx=30)
    my_entries.append(my_entry)
for x in range(3):
    my_entry = Entry(root)
    my_entry.insert(END, '')
    my_entry.grid(row=6, column=x, pady=10)
    my_entries.append(my_entry)
for x in range(3):
    my_entry = Entry(root)
    my_entry.insert(END, '')
    my_entry.grid(row=10, column=x, pady=10)
    my_entries.append(my_entry)
for x in range(1):
    my_entry = Entry(root)
    my_entry.insert(END, '0')
    my_entry.grid(row=15, column=1, pady=10)
    my_entries.append(my_entry)
    my_label1=Label()
    my_label1.grid(row=16, column=1)
    my_label1 = Label(root, text='For Forward Kinematic = 1 & For Inverse Kinematik = 0 ')
    my_label1.grid(row=16, column=1)



my_button = Button(root, text="Movement", command=Fill_in_Data)
my_button.grid(row=11, column=1, pady=10)

my_label1= Label(root, text='Inverse Kinematics')
my_label1.grid(row=0, column=1, pady=10)
my_label1= Label(root, text='X')
my_label1.grid(row=1, column=0, pady=10)
my_label1= Label(root, text='Y')
my_label1.grid(row=1, column=1, pady=10)
my_label1= Label(root, text='Z')
my_label1.grid(row=1, column=2, pady=10)

my_label2= Label(root, text='Forward Kinematic')
my_label2.grid(row=3, column=1, pady=10)
my_label2= Label(root, text='JOG1 between -150 to +150')
my_label2.grid(row=4, column=0, pady=10)
my_label2= Label(root, text='JOG2 between -100 to +100')
my_label2.grid(row=4, column=1, pady=10)
my_label2= Label(root, text='JOG3 between -100 to +100')
my_label2.grid(row=4, column=2, pady=10)

my_label3= Label(root, text='Features')
my_label3.grid(row=8, column=1, pady=10)
my_label3= Label(root, text='Gripper Value')
my_label3.grid(row=9, column=0, pady=10)
my_label3= Label(root, text='FUNCTION EMPTY')
my_label3.grid(row=9, column=1, pady=10)
my_label3= Label(root, text='Homing')
my_label3.grid(row=9, column=2, pady=10)


"""" #making sliders for Jogging

def Base_motor(var):
    JOG1=var+','
    ser.write(JOG1.encode())
    print(JOG1)

def Middle_Motor(var):
    JOG2=var+','
    ser.write(JOG2.encode())

def Last_Motor(var):
    JOG3=var+','
    ser.write(JOG3.encode())



Base_Motor = Scale(root, from_=-10, to=10, orient=HORIZONTAL, command=Base_motor)
Base_Motor.grid(pady=0)
my_label2= Label(root, text='JOG1')
my_label2.grid(padx=20)
Middle_Motor = Scale(root, from_=0, to=200, orient=HORIZONTAL, command=Middle_Motor)
Middle_Motor.grid(pady=0)
my_label3= Label(root, text='JOG2')
my_label3.grid(padx=20)
Last_Motor= Scale(root, from_=0, to=200, orient=HORIZONTAL, command=Last_Motor)
Last_Motor.grid(pady=0)
my_label2= Label(root, text='JOG3')
my_label2.grid(padx=30)

Base_Motor.get()
Middle_Motor.get()
Last_Motor.get() """

root.mainloop()
