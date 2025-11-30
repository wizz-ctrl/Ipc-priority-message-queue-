#!/bin/bash
# GitHub Push Helper Script
# Makes it easy to push updates to GitHub

echo ""
echo "╔════════════════════════════════════════════════╗"
echo "║     GitHub Push Helper                         ║"
echo "╚════════════════════════════════════════════════╝"
echo ""

# Check if git is initialized
if [ ! -d .git ]; then
    echo "❌ Error: Not a git repository!"
    echo "Run: git init"
    exit 1
fi

# Check if remote is set
if ! git remote | grep -q origin; then
    echo "⚠️  No remote repository found!"
    echo ""
    echo "Please add your GitHub repository:"
    echo "  git remote add origin https://github.com/YOUR_USERNAME/ipc-priority-message-queue.git"
    echo ""
    read -p "Enter your GitHub repository URL: " repo_url
    if [ ! -z "$repo_url" ]; then
        git remote add origin "$repo_url"
        echo "✅ Remote added!"
    else
        echo "❌ Cancelled"
        exit 1
    fi
fi

# Show current status
echo "📊 Current Status:"
git status --short
echo ""

# Ask for commit message
read -p "📝 Enter commit message (or press Enter for default): " commit_msg
if [ -z "$commit_msg" ]; then
    commit_msg="Update: $(date '+%Y-%m-%d %H:%M')"
fi

# Add all changes
echo ""
echo "📦 Adding changes..."
git add .

# Commit
echo "💾 Committing..."
git commit -m "$commit_msg"

# Check if commit was successful
if [ $? -eq 0 ]; then
    echo "✅ Committed successfully!"
    echo ""
    
    # Ask to push
    read -p "🚀 Push to GitHub? (y/n): " push_confirm
    if [ "$push_confirm" = "y" ] || [ "$push_confirm" = "Y" ]; then
        echo "📤 Pushing to GitHub..."
        git push -u origin master
        
        if [ $? -eq 0 ]; then
            echo ""
            echo "╔════════════════════════════════════════════════╗"
            echo "║     ✅ Successfully pushed to GitHub!          ║"
            echo "╚════════════════════════════════════════════════╝"
            echo ""
            echo "View your repository:"
            git remote get-url origin | sed 's/\.git$//'
        else
            echo ""
            echo "❌ Push failed!"
            echo "Check your credentials or internet connection"
        fi
    else
        echo "⏭️  Skipped push"
    fi
else
    echo "❌ Nothing to commit or commit failed"
fi

echo ""
