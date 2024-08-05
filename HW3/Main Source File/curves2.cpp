#include "WorkingScene.h"

// This file includes the basic functions that your program must fill in.  
// Your assignment consists of filling in parts that say /* YOUR CODE HERE */
// There are many helpful functions that can be found in Curve.cpp! In particular,
// take a look at moveActivePoint, drawLine, connectTheDots, etc.

// What happens when you drag the mouse to x and y?  
// In essence, you are dragging control points on the curve.
void WorkingScene::drag(int x, int y) {
    //you must figure out how to transform x and y so they make sense
    int distanceX = x - oldx;
    int distanceY = oldy - y;
    
    float normX = distanceX / (float) width;
    float normY = distanceY / (float) height;
    
    theOnlyCurve->moveActivePoint(normX, normY);

	//update oldx, and oldy
    oldx = x;
    oldy = y;
    
	//make sure scene gets redrawn
    glutPostRedisplay();
}

// Mouse motion.  You need to respond to left clicks (to add points on curve) 
// and right clicks (to delete points on curve) 
void WorkingScene::mouse(int button, int state, int x, int y) {
    float normX = float(x) / Scene::width;
    float normY = (Scene::height - float(y)) / Scene::height;
    
	if (theOnlyCurve && state == GLUT_DOWN) {
		if (button == GLUT_LEFT_BUTTON) {
            theOnlyCurve->addPoint(normX, normY);
		}
		if (button == GLUT_RIGHT_BUTTON) {
            theOnlyCurve->updateActivePoint(normX, normY);
            theOnlyCurve->deleteActivePoint();
		}
	}

    //update oldx, and oldy
    oldx = x;
    oldy = y;
    
	//make sure scene gets redrawn
    glutPostRedisplay();
}

#include "Bezier.h"

// Bezier drawing function.  This is by deCasteljau or equivalent algorithm. 
// It should support Bezier curves of arbitrary degree/order.
void Bezier::draw(int levelOfDetail) {

	connectTheDots();
	int i,j,k;
	Pvector::iterator p;
    
    if(points.empty() || points.size() == 1) {
        return;
    }
    
    // Points for the Bezier curve
    Pvector BezierPoints;
    
    // Obtain initial control point list P
    Pvector PList = points;
    
    // Obtain each level of detail
    vector<float> u;
    
    for (int i = 0; i <= levelOfDetail; i++) {
        u.push_back(i / (float) levelOfDetail);
    }
    
    // Loop through each level of detail
    for (int j = 0; j < u.size(); j++) {
        
        float t = u[j];
        
        Pvector P_currList = PList; // the current list of control points
        
        // Loop through each of the original control points
        for (int k = 0; k < PList.size() - 1; k++) {   // May have to change PList.size()
            
            Pvector P_newList; // the new list of control points in a new level
            
            for (p = P_currList.begin(); p != P_currList.end() - 1; p++) {
                // Calculate new control points of current level
                float x = t * (p->x) + (1-t) * ((p+1)->x); //calculate x
                float y = t * (p->y) + (1-t) * ((p+1)->y); //calculate y
                P_newList.push_back(Point(x, y));
            }
            
            // update P_currList
            P_currList = P_newList;
    
        }
        // add the control points of only the highest level for curve drawing
        BezierPoints.push_back(P_currList[0]);
    }
    
    // Draw the Bezier curve
    for (p = BezierPoints.begin(); p != BezierPoints.end() - 1; p++) {
        drawLine(p->x, p->y, (p+1)->x, (p+1)->y);
    }
}

#include "Bspline.h"

// The B-Spline drawing routine.  
// Remember to call drawSegment (auxiliary function) for each set of 4 points.
void Bspline::draw(int levelOfDetail) {


	connectTheDots();
	
    int drawFour = 1;
    Pvector::iterator p1, p2, p3, p4;
    
    // Draw a segment for each set of 4 points
    for (Pvector::iterator p = points.begin(); p != points.end(); p++) {
        p1 = p2;
        p2 = p3;
        p3 = p4;
        p4 = p;
        
        if(drawFour > 3) {
            drawSegment(p1, p2, p3, p4, levelOfDetail);
        }
        
        drawFour++;
    }
}

void Bspline::drawSegment(Pvector::iterator p1, Pvector::iterator p2, 
		Pvector::iterator p3, Pvector::iterator p4, int levelOfDetail) {

	float x, y;
    
    // Obtain each level of detail
    vector<float> levels;
    
    for (int i = 0; i <= levelOfDetail; i++) {
        levels.push_back(i / (float) levelOfDetail);
    }

	//draw segment using explicit matrix form
    float xold = (1/6.0f) * ((float)((p1->x) + 4*(p2->x) + (p3->x)));
    float yold = (1/6.0f) * ((float)((p1->y) + 4*(p2->y) + (p3->y)));
    
    Point(xold, yold).draw();
    
    for (int i = 0; i < levels.size(); i++) {
        float u = levels[i];
        
        x = (u * u * u) * ((1.0f/6.0f) * (float)( -(p1->x)  + 3*(p2->x) - 3*(p3->x) + (p4->x)))
          + (u * u)     * ((1.0f/6.0f) * (float)( 3*(p1->x) - 6*(p2->x) + 3*(p3->x)          ))
          + (u)         * ((1.0f/6.0f) * (float)( -3*(p1->x)            + 3*(p3->x)          ))
          +  1.0f       * ((1.0f/6.0f) * (float)( (p1->x)   + 4*(p2->x) + (p3->x)            ));
        
        y = (u * u * u) * ((1.0f/6.0f) * (float)( -(p1->y)  + 3*(p2->y) - 3*(p3->y) + (p4->y)))
          + (u * u)     * ((1.0f/6.0f) * (float)( 3*(p1->y) - 6*(p2->y) + 3*(p3->y)          ))
          + (u)         * ((1.0f/6.0f) * (float)( -3*(p1->y)            + 3*(p3->y)          ))
          +  1.0f       * ((1.0f/6.0f) * (float)( (p1->y)   + 4*(p2->y) + (p3->y)            ));
        
        drawLine(xold, yold, x, y);
        xold = x;
        yold = y;
    }

	//then create a Point to be drawn where the knot should be
	Point p(x, y);
	p.draw();
}

#include "Bezier2.h"

//This function is provided to aid you.
//It should be used in the spirit of recursion, though you may choose not to.
//This function takes an empty vector of points, accum
//It also takes a set of control points, pts, and fills accum with
//the control points that correspond to the next level of detail.
void accumulateNextLevel(Pvector* accum, Pvector pts) {
	if (pts.empty()) return; 
	accum->push_back(*(pts.begin()));
	if (pts.size() == 1) return;
	for (Pvector::iterator it = pts.begin(); it != pts.end() - 1; it++) {
        float x = ((1/2.0f)*((it->x) + ((it+1)->x)));
        float y = ((1/2.0f)*((it->y) + ((it+1)->y)));
        *it = Point(x, y);
	}
	//save the last point
	Point last = *(pts.end()-1);
	pts.pop_back();
	//recursive call
	accumulateNextLevel(accum, pts);
	accum->push_back(last);
}

// The basic draw function for Bezier2.  Note that as opposed to Bezier, 
// this draws the curve by recursive subdivision.  So, levelofdetail 
// corresponds to how many times to recurse.  
void Bezier2::draw(int levelOfDetail) {
	//This is just a trick to find out if this is the top level call
	//All recursive calls will be given a negative integer, to be flipped here
	if (levelOfDetail > 0) {
		connectTheDots();
	} else {
		levelOfDetail = -levelOfDetail;
	}

	//Base case.  No more recursive calls.
	if (levelOfDetail <= 1) {
		if (points.size() >= 2) {
			for (Pvector::iterator it = points.begin(); it != points.end() - 1; it++) {
                drawLine((it->x),(it->y),((it+1)->x),((it+1)->y));
			}
		}
	} else {
		Pvector* accum = new Pvector();
		Bezier2 left, right;

		//add the correct points to 'left' and 'right'.
		//You may or may not use accum as you see fit.
        accumulateNextLevel(accum, points);
        Pvector::iterator a = (*accum).begin();
        
        int size = (*accum).size();
        
        for (int i = 1; i <= size; i++) {

        // Left side
        if ((i <= size / 2) || (((size % 2) == 1) && (i == ((size / 2) + 1)))) {
            left.points.push_back(*a);
        }
        // Right side
        if (i > (size / 2)) {
            right.points.push_back(*a);
        }
            a++;
        }

		left.draw(1-levelOfDetail);
		right.draw(1-levelOfDetail);
		delete accum;
	}
}
