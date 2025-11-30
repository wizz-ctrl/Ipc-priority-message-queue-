# 🎯 SIMPLE 3-STEP GITHUB UPLOAD GUIDE

## Your code is READY! Just follow these 3 simple steps:

---

## 📌 STEP 1: Create GitHub Repository (2 minutes)

### 1. Open GitHub
Go to: **https://github.com**

### 2. Click "New Repository" 
- Look for the **"+"** button (top-right corner)
- Click **"New repository"**

### 3. Fill in these details:

```
┌─────────────────────────────────────────────────┐
│ Repository name*                                │
│ ipc-priority-message-queue                      │
└─────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────┐
│ Description (optional)                          │
│ Advanced IPC Priority Message Queue - Linux     │
│ Kernel Module (OS Assignment 2)                 │
└─────────────────────────────────────────────────┘

☑️ Public  ☐ Private    ← CHOOSE PUBLIC!

☐ Add a README file        ← Leave UNCHECKED
☐ Add .gitignore          ← Leave UNCHECKED
☐ Choose a license        ← Optional: GPL v3.0
```

### 4. Click "Create repository" (green button)

---

## 📌 STEP 2: Connect Your Code (30 seconds)

GitHub will show you a page with commands. **Ignore them!** Use these instead:

### Open terminal and run:

```bash
cd /home/taimoor-safdar/os-assignment2

# Replace YOUR_USERNAME with your actual GitHub username!
git remote add origin https://github.com/YOUR_USERNAME/ipc-priority-message-queue.git

# Check it worked
git remote -v
```

**Example:** If your username is `taimoor123`, use:
```bash
git remote add origin https://github.com/taimoor123/ipc-priority-message-queue.git
```

---

## 📌 STEP 3: Upload Your Code (30 seconds)

### Run this command:

```bash
git push -u origin master
```

### If it asks for credentials:

```
Username: your_github_username
Password: [Use Personal Access Token, not your password!]
```

### Don't have a token? Create one:

1. Go to: **https://github.com/settings/tokens**
2. Click **"Generate new token"** → **"Generate new token (classic)"**
3. Name it: **"OS Assignment Upload"**
4. Check the **"repo"** checkbox
5. Click **"Generate token"** (bottom)
6. **COPY THE TOKEN** (you'll only see it once!)
7. Use this token as your password when pushing

---

## ✅ STEP 4: Verify It Worked!

### Open your browser:
```
https://github.com/YOUR_USERNAME/ipc-priority-message-queue
```

**You should see:**
- ✅ All your code files
- ✅ README displayed
- ✅ Green "Code" button
- ✅ File list on the left

---

## 🎉 DONE! Now What?

### 1. Copy Your Repository URL:
```
https://github.com/YOUR_USERNAME/ipc-priority-message-queue
```

### 2. Add it to your assignment submission!

### 3. (Optional) Make it look nicer:

#### Add topics:
- Click **"⚙️"** next to "About"
- Add: `linux` `kernel-module` `ipc` `operating-systems` `c`

#### Add description:
- Click **"⚙️"** next to "About"  
- Paste: "Advanced IPC Priority Message Queue - Linux Kernel Module (OS Fall 2025)"

---

## 🔧 Common Problems & Quick Fixes

### ❌ "fatal: remote origin already exists"
**Fix:**
```bash
git remote remove origin
git remote add origin https://github.com/YOUR_USERNAME/ipc-priority-message-queue.git
```

### ❌ "Authentication failed"
**Fix:** You need a Personal Access Token (see Step 3 above)

### ❌ "Repository not found"
**Fix:** Double-check:
1. Repository name is exactly: `ipc-priority-message-queue`
2. You replaced YOUR_USERNAME with your actual username
3. Repository is created on GitHub

### ❌ "Permission denied"
**Fix:** Make sure repository is Public, not Private

---

## 📱 Quick Commands Cheat Sheet

```bash
# Navigate to project
cd /home/taimoor-safdar/os-assignment2

# Add remote (do once)
git remote add origin https://github.com/YOUR_USERNAME/ipc-priority-message-queue.git

# Push code (do once)
git push -u origin master

# Future updates (easy way)
./push_to_github.sh

# Or manually
git add .
git commit -m "Updated XYZ"
git push
```

---

## 💡 Pro Tips

1. **Star your own repository** - Shows you're proud of your work! ⭐

2. **Share the link** with your group members

3. **Take a screenshot** of your GitHub repo for your report

4. **Keep it updated** - If you make changes, use `./push_to_github.sh`

---

## ✅ Checklist Before Submission

- [ ] Repository is created on GitHub
- [ ] Code is pushed and visible
- [ ] Repository is PUBLIC (not private)
- [ ] README displays properly
- [ ] You can open the repository URL in a browser
- [ ] Repository URL is included in assignment submission

---

## 🆘 Still Stuck?

### Read the detailed guide:
```bash
cat GITHUB_SETUP.md
```

### Or ask for help:
- Your group members
- TA during office hours
- Course discussion forum

---

## 🎯 Your Goal

**Final URL should look like:**
```
https://github.com/your_username/ipc-priority-message-queue
```

**That's it! Simple as 1-2-3!** 🚀

---

<div align="center">

**Remember:** Replace `YOUR_USERNAME` with your actual GitHub username!

**Good luck!** 🎉

</div>
