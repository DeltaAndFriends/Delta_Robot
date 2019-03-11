


		std::vector<int> i_vals = {0, 2, 4};
		for (auto i : i_vals) {
			tri_vector_1 = Matrix({ {coord[i].x}, {coord[i].y}, {coord[i].z}, {1} }, 4, 1);
			++i;
			tri_vector_2 = Matrix({ {coord[i].x}, {coord[i].y}, {coord[i].z}, {1} }, 4, 1);
			--i;

			if (DEBUG) {
				std::cout << "Iteration "<< i <<":" << std::endl;
				chapter("TRI_VECTORS 1 and 2");
				std::cout << tri_vector_1 << std::endl << std::endl
					<< tri_vector_2 << std::endl << std::endl;
			}

			point_1 = pre_calc * tri_vector_1;
			point_2 = pre_calc * tri_vector_2;

			if (DEBUG) {
				chapter("POINTS 1 and 2");
				std::cout << point_1 << std::endl << std::endl
					<< point_2 << std::endl << std::endl;
			}

			for (size_t j = 0; j < 3; j++) {// finding middle values and filling them in array
				// assert(i/2 < 3);
				middles[i / 2][j] = mid(point_1[{i, j}], point_2[{i, j}]) - intersection[i / 2][j]; // while filling, we move origin of the coordinate system
				// if (DEBUG) {
				// 	std::cout << "middles[" << i / 2 << "][" << j << "] = mid("
				// 		<< point_1[{i, j}] << "," << point_2[{i, j}] << ") - intersection["
				// 		<< i / 2 << "][" << j << "];" << std::endl;
				// 	std::cout << "  middles: " << middles[i / 2][j] << std::endl
				// 		<< "  mid: " << mid(point_1[{i, j}], point_2[{i, j}]) << std::endl
				// 		<< "  intersection: " << intersection[i / 2][j] << std::endl;
				// }
			}

			if (DEBUG) {
				chapter("MIDDLE POINT COORDINATES");
				std::cout << middles[i][0] << std::endl
					<< middles[i][1] << std::endl
					<< middles[i][2] << std::endl;
			}

		}


		double destination[3]; // where to store found angles


		// Step 2. Finding Xb and Zb. (Yb is definitely 0) with
		// Step 3. Finding angles for Xb and Zb.

		int64_t K = sizes.y*sizes.y - sizes.z*sizes.z;
		if (DEBUG) {
			chapter("K = y^2 - z^2 // precalculation");
			std::cout << "sizes.y: " << sizes.y << std::endl
				<< "sizes.z: " << sizes.z << std::endl
				<< "K:" << K << std::endl;
		}


		long double xB[3];
		long double zB[3];

		long double D = 0;
		const double &h = sizes.z;

		for (short i = 0; i < 3; i++) {
			// Step 2:
			const double &l = middles[i][0];
			const double &m = middles[i][1];
			const double &n = middles[i][2];

			// x*x usually isn't more effective than Math.pow(x, 2), but in some cases may be.
			K -= l * l + m * m + n * n;
			K /= 2; // Bit operations are unneeded, because the compiler will optimize that for us

			if (DEBUG) {
				chapter("K -= l^2 + m^2 + n^2, K /= 2 // IN LOOP");
				std::cout << "l: " << l << std::endl
					<< "m: " << m << std::endl
					<< "n: " << n << std::endl
					<< "h: " << h << std::endl
					<< "K:" << K << std::endl;
			}

			D = K * K*n*n;
			D -= (n*n + l * l)*(K*K - l * l*h*h);

			if (DEBUG) {
				chapter("D before sqrt: ");
				std::cout << "D: " << D << std::endl;
			}

			D = sqrt((long double)D);

			if (DEBUG) {
				chapter("D = sqrt(K*K*n*n - (n*n + l*l)*(K*K - l*l*h*h)) // IN LOOP");
				std::cout << "K*K: " << K * K << std::endl
					<< "K*K*n*n: " << K * K*n*n << std::endl
					<< "(n*n + l*l): " << (n*n + l * l) << std::endl
					<< "(K*K - l*l*h*h): " << (K*K - l * l*h*h) << std::endl
					<< "D: " << D << std::endl;
			}

			xB[i] = (D - K * l) / 2 * (n*n + l * l);
			zB[i] = sqrt(h*h - xB[i] * xB[i]);

			if (DEBUG) {
				chapter("xB and zB  // LOOP");
				std::cout << "xB = (D - K*l) / (n^2 + l^2) // IN LOOP"
					<< std::endl << std::endl;
				std::cout << "-K*l: " << -K * l << std::endl
					<< "D: " << D << std::endl
					<< "(n*n + l*l): " << (n*n + l * l) << std::endl
					<< "xB: " << xB[i] << std::endl;

				std::cout << "zB = sqrt(h^2 - xB^2); // IN LOOP"
					<< std::endl << std::endl;
				std::cout << "h^2: " << h * h << std::endl
					<< "xB^2: " << xB[i] * xB[i] << std::endl
					<< "zB: " << zB[i] << std::endl;
			}

			system("pause");
			// Step 3:
			destination[i] = xB[i];
			destination[i] /= sqrt(((long double)xB[i] * xB[i] + (long double)zB[i] * zB[i]));
			destination[i] = acos(destination[i]);

			if (DEBUG) {
				chapter("DEST = xB;\nDEST /= sqrt(xB^2 + zB^2);\nDEST = acos(DEST) // IN LOOP");
				std::cout << "xB: " << xB[i] << std::endl
					<< "sqrt(xB^2 + zB^2): " << sqrt(((long double)xB[i] * xB[i] + (long double)zB[i] * zB[i])) << std::endl
					<< "acos(DEST): " << destination[i] << std::endl << std::endl;
			}
		}
		triplet dest{ destination[0], destination[1], destination[2] };
		return dest;
	}
}
