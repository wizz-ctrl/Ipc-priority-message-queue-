# 📝 GitHub Repository Setup Guide

## Step-by-Step Instructions to Upload Your Project

---

## 🌐 Step 1: Create GitHub Repository

### Option A: Using GitHub Website (Recommended)

1. **Go to GitHub**
   - Open your browser: https://github.com
   - Sign in to your account (or create one if you don't have it)

2. **Create New Repository**
   - Click the **"+"** icon in the top-right corner
   - Select **"New repository"**

3. **Configure Repository**
   ```
   Repository name: ipc-priority-message-queue
   Description: Advanced IPC Priority Message Queue - Linux Kernel Module (OS Assignment 2)
   
   ☑️ Public (so it can be graded)
   ☐ Add a README file (we already have one)
   ☐ Add .gitignore (we already have one)
   ☐ Choose a license: GNU General Public License v3.0
   ```

4. **Click "Create repository"**

---

## 🔗 Step 2: Link Your Local Repository to GitHub

After creating the repository, GitHub will show you commands. Use these:

### Copy your repository URL
It will look like: `https://github.com/YOUR_USERNAME/ipc-priority-message-queue.git`

### Run these commands in your terminal:

```bash
cd /home/taimoor-safdar/os-assignment2

# Add the remote repository
git remote add origin https://github.com/YOUR_USERNAME/ipc-priority-message-queue.git

# Verify remote was added
git remote -v
```

---

## 📤 Step 3: Push Your Code to GitHub

```bash
# Push to GitHub
git push -u origin master
```

**If prompted for credentials:**
- Username: Your GitHub username
- Password: Use a **Personal Access Token** (not your GitHub password)

### How to Create Personal Access Token (if needed):

1. Go to GitHub Settings: https://github.com/settings/tokens
2. Click **"Generate new token"** → **"Generate new token (classic)"**
3. Give it a name: "OS Assignment Upload"
4. Select scopes: ☑️ **repo** (all sub-items)
5. Click **"Generate token"**
6. **COPY THE TOKEN** (you won't see it again!)
7. Use this token as your password when pushing

---

## ✅ Step 4: Verify Upload

1. Go to your GitHub repository URL
2. You should see all your files:
   - safe_lkm.c
   - Makefile
   - Test files
   - Documentation files
   - README_GITHUB.md

---

## 🎨 Step 5: Set GitHub README (Optional but Recommended)

To use the nicer GitHub README as the main README:

### Option 1: Via GitHub Website
1. Go to your repository on GitHub
2. Click on `README_GITHUB.md`
3. Click the pencil icon (Edit)
4. Copy all content
5. Go back to repository main page
6. Click on `README.md`
7. Click pencil icon (Edit)
8. Replace content with content from README_GITHUB.md
9. Click "Commit changes"

### Option 2: Via Command Line
```bash
cd /home/taimoor-safdar/os-assignment2

# Backup current README
mv README.md README_LOCAL.md

# Use GitHub README as main README
cp README_GITHUB.md README.md

# Commit and push
git add README.md README_LOCAL.md
git commit -m "Update README for GitHub display"
git push
```

---

## 📋 Step 6: Add Repository Information to Your Submission

Your GitHub repository URL will be:
```
https://github.com/YOUR_USERNAME/ipc-priority-message-queue
```

Include this URL in your assignment submission!

---

## 🔧 Common Issues & Solutions

### Issue 1: "Permission denied"
**Solution:** Make sure you're using a Personal Access Token, not your password

### Issue 2: "Repository not found"
**Solution:** Check that the repository name matches exactly:
```bash
git remote -v  # Check current remote
git remote remove origin  # Remove if wrong
git remote add origin https://github.com/YOUR_USERNAME/ipc-priority-message-queue.git
```

### Issue 3: "Authentication failed"
**Solution:** 
1. Create a Personal Access Token (see Step 3)
2. Or use SSH key instead:
   ```bash
   ssh-keygen -t ed25519 -C "your_email@example.com"
   cat ~/.ssh/id_ed25519.pub  # Copy this to GitHub Settings > SSH Keys
   git remote set-url origin git@github.com:YOUR_USERNAME/ipc-priority-message-queue.git
   ```

---

## 📊 What's Already Done ✅

Your local repository is already set up with:
- ✅ Git initialized
- ✅ .gitignore configured
- ✅ All source files added
- ✅ Initial commit made
- ✅ GitHub-ready README created

**You just need to:**
1. Create the GitHub repository (Steps 1-2)
2. Push the code (Step 3)

---

## 🎯 Quick Command Summary

```bash
# 1. Create repository on GitHub website first!

# 2. Link your local repo (replace YOUR_USERNAME)
cd /home/taimoor-safdar/os-assignment2
git remote add origin https://github.com/YOUR_USERNAME/ipc-priority-message-queue.git

# 3. Push your code
git push -u origin master

# 4. (Optional) If you make changes later
git add .
git commit -m "Description of changes"
git push
```

---

## 📸 What Your GitHub Page Will Look Like

Your repository will display:
- 📁 All source files and documentation
- 📖 README with nice formatting and badges
- 🎨 Syntax-highlighted code
- 📊 Contribution graph
- ⭐ Star/fork buttons

Example: https://github.com/example/ipc-priority-message-queue

---

## 🏆 Pro Tips

1. **Make it Public** - So your instructor can access it
2. **Add Topics** - Click "⚙️ Settings" → Add topics: `linux`, `kernel-module`, `ipc`, `operating-systems`
3. **Add Description** - "Advanced IPC Priority Message Queue - Linux Kernel Module"
4. **Star Your Own Repo** - Shows you care about your work! 😄

---

## 📞 Need Help?

If you get stuck:
1. Check GitHub's official guide: https://docs.github.com/en/get-started
2. The TA or instructor can help with GitHub-specific issues
3. Your classmates might have solved similar issues

---

## ✅ Verification Checklist

After pushing, verify:
- [ ] Repository is public
- [ ] All files are visible
- [ ] README displays properly
- [ ] You can access the repository URL in a browser
- [ ] Code is syntax-highlighted
- [ ] Repository URL is included in your assignment submission

---

**You're ready to push to GitHub! 🚀**

Follow Steps 1-3 above, and your assignment will be online!
