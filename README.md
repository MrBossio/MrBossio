### Hello world 👋
This Github is created with the purpose of being a notebook in my process to become a good BackEnd/DevOps, and also a kind of guide for new people who wants to learn some topics about development.  
I am Marco, an Informatic Engineer graduated from the "Pontificia Universidad Católica del Perú - PUCP", specialized in Cybersecurity, Backend & DevOps.  
I also have extensive knowledge of videogames/2D/3D animation by my first degree in Graphic Design. 

### About the learning path
I am using https://roadmap.sh/devops as reference for languages/technologies/tools.  
I will be sharing some projects/demos/PoC related to every topic in the roadmap.  
Let's get started!

## 1: Programming languages ​​that I use in my projects (Java, Python, others)
<p align="center" size="small">
  <a href="https://skillicons.dev">
    <img src="https://skillicons.dev/icons?i=python,django,java,spring,c,cpp,mysql,postgres,postman" />
  </a>
</p>
We are developers, obviously, we need to learn some languages to work in IT. I have worked with many languages and I can recommend two: Python and Java. I also recommend to learn the most commercial framework for every language. Finally, you need to know SQL to manage databases.      
<br/><br/>  

`Java17` `Springboot` `MySQL8`  
- My projects:
	- **[Air-cargo collapse simulator](https://github.com/MrBossio/parcel-shipments-by-air-cargo-API)**: A program to simulate the daily operations of an Air-cargo company, trying to determine the most efficient algorithm to avoid logistics collapse. Please read carefully the readme file for full details.  

`Python` `Django` `PostgreSQL` `Zappa` `AWS` `Lambda` 
- My projects:
	- **[Django deployment in AWS Lambda](https://github.com/MrBossio/sigcc-back)**: Running a serverless Django project in AWS Lambda: A example project to run a Django backend using AWS Lambdas, deployed with Zappa. Capstone project to opt for my Informatics Engineering Degree. For this project I had the role of Tech Leader, main commit reviewer and DevOps, leading a group of 13 developers.

`C` `C++` `C#` 
- My projects:
	- **[LegadoDelSol_v1.0](https://github.com/MrBossio/LegadoDelSol_v1.0)**: Development of a historical fiction video game based on the history of Inca Peru using the User Centered Design framework that generates interest in the activities addressed in the video game. Thesis to opt for my Informatics Engineering Degree. [Download and play](https://drive.google.com/open?id=1-Fqzb5YTq8j-yqrTKmPvPIrYu_TdAMMk&usp=drive_fs).
	- **[ComputerGraphicArts](https://github.com/MrBossio/ComputerGraphicArts)**: 2D and 3D graphic arts using C++ with openGL GLUT library. All the images/videos were written in code.

> [!NOTE]  
> <sub>Java is the standard of the industry. There is a lot of legacy code based in Java, and also new developments related to this language. Is a must in your list of languages.  
> Python is a great language for a big range of uses, and the main language in Data science. It has very useful libraries for everything.  
> C and C++ (and for extension C#) are some of the foundations of our modern society. Is quite difficult to learn C and derivates, but very helpful to become a good developer because you will learn to manage the computer resources at a very low level.</sub>  


## 2: Learning about Operative Systems, terminal and bash scripting

<p align="center">
  <a href="https://skillicons.dev">
    <img src="https://skillicons.dev/icons?i=windows,redhat,ubuntu,linux,bash,powershell,nginx,c" />
  </a>
</p>

Since most of my code run in servers, I needed to learn the basis (and some advanced topics) to deploy successfully my developed applications. 

`Bash` `Linux`
- Some projects:
	- **[How to config rsync to syncronize and backup directories]()**: A quick tutorial to syncronize two folders in different Linux servers for a simple backup in real time.	
	- **[How to create/renew certificates in a Linux/Windows server]()**: Some notes to remember how to manage expired certificates in servers.  

`Bash` `Linux` `Nginx` `Docker Swarm`
- Some projects:
    - **[Demo project, configure a reverse proxy to deploy applications using Nginx and Docker]()**: A example project where I deploy two dockerized applications using Nginx and Docker Swarm. 

`Bash` `C`
- Some old reference documents:
    - **[Linux commands and C system calls](Linux_commands_and_C_system_calls/Linux_commands_and_C_system_calls.md)**: Some student notes on Linux terminal commands and C system calls. They are in Spanish (my native language, and probably doesn't contain the exact description, but they were very useful to me at the time).  

## 3: Working in the clouds

<p align="center">
  <a href="https://skillicons.dev">
    <img src="https://skillicons.dev/icons?i=aws,azure,gcp,dynamodb" />
  </a>
</p>


Hey, Did I tell you that I have extensive experience in the cloud? I have two certifications in AWS (please click to open the credly badget):  
	- **[AWS Certified Developer – Associate (DVA-C01)](https://www.credly.com/badges/88c50bd8-dc55-4a7b-9d11-7c3da6d5c149/public_url)**  
	- **[AWS Certified Data Analytics – Specialty (DAS-C01)](https://www.credly.com/badges/565789e9-78f0-440a-be47-1a708322e796/linked_in)**   
Currently, I have extensive knowledge about AWS cloud, using serverless architecture, noSQL, IaC (CloudFormation), IAM and more.  

Also, I am working to obtain the AZ-204 of Azure, and learning new technologies to automatize deployment in multicloud platforms (Terraform/Ansible).

`Python` `Django` `PostgreSQL` `Zappa` `AWS` `Lambda` 
- My projects:
	- **[Django deployment in AWS Lambda](https://github.com/MrBossio/sigcc-back)**: Running a serverless Django project in AWS Lambda: A example project to run a Django backend using AWS Lambdas, deployed with Zappa. Capstone project to opt for my Informatics Engineering Degree. For this project I had the role of Tech Leader, main commit reviewer and DevOps, leading a group of 13 developers.

`Cloudformation` `AWS` `CDK` `Python` `Lambda` `DynamoDB` `Cognito` `ApiGateway` 
- My projects:
	- **[IaC in Cloudformation to deploy an API using serverless Lambda, DynamoDB and Cognito]()**: Using CloudFormation, I deployed a project to expose an API with auth and token rotation (Cognito/Api Gateway), running in serverless (Lambda), connected to a DynamoDB noSQL database.


## 4: About repositories (Version Control Systems), Documentation and Project Management

<p align="center">
  <a href="https://skillicons.dev">
    <img src="https://skillicons.dev/icons?i=git,bitbucket,github,markdown,obsidian" />
  </a>
</p>

To work in IT/Development, we need to know how to work with at least a VCS repository, a documentation tool and a tool for project management.

1. **Project Management**: I have worked mainly with Jira, since this is a industry standar. Also Jira has the facility to connect to BitBucket (you can link taks with branches in your code) and Confluence (where you can document your code, and easily show your files directly from BitBucket). I also worked with ClickUp.

2. **Documentation**: You have at your disposition several tools to document your projects. Obiously, it depends of the tool you choose.  
	1. GitHub: you will need to use Markdown to document your project, similar to this document you are currently reading.
	2. Confluence: you can also document using the Atlassian main tool. This have a lot of tools to enrich your documentation (graphs, code styles, links to BitBucket/Jira). As a disadvantage, this tool is not fully compatible with Markdown.
	3. Obsidian: a beautyfull tool to write documentation. This is open source, and can run in local for free or hosted if you pay a fee. As Confluence, this is not fully compatible with Markdown.  

	`Obsidian`
	- Some reference documents:
		- **[Learning notes from course "Ansible: From Basics To Guru"](https://github.com/MrBossio/Learning-notes-Ansible)**: Some student notes on Linux terminal commands and C system calls. They are in Spanish (my native language, and probably doesn't contain the exact description, but they were very useful to me at the time).  

3. **Version Control Systems**: I have worked with many VCS tools:
	- BitBucket: I have worked mainly with this tool because this is related to Jira. I have configured repositories to connect Jira epics and taks to BitBucket branches, and also configured roles and permissions to modify/push/review code in multiple tier level branches.
	- Github: Obviously I have worked with Github, since this is another industry standar.

## 5: Containerizing applications for development and production

<p align="center">
  <a href="https://skillicons.dev">
    <img src="https://skillicons.dev/icons?i=docker,kubernetes,nginx" />
  </a>
</p>


## 6: Working with CI/CD tools and Configuration Management

<p align="center">
  <a href="https://skillicons.dev">
    <img src="https://skillicons.dev/icons?i=jenkins,ansible,terraform" />
  </a>
</p>
